#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv2/imgproc.hpp>
#include <deque>

void captureFrame(cv::VideoCapture* cap, cv::Mat *frame) {
    cap->read(*frame);
    if (frame->empty()) {
        std::cerr << "ERROR! blank frame grabbed" << std::endl;
    }
}

void matchCircles(std::vector<cv::Vec3f> *circles, cv::Mat *frame) {
    cv::Mat preparedFrame;
    std::vector<cv::Vec3f> tmpCircles;

    //prepare frame
    cv::cvtColor(*frame, preparedFrame, 6);
    cv::medianBlur(preparedFrame, preparedFrame, 5);
    cv::Mat edges;
    Canny(preparedFrame, edges, 100, 200, 5, false);
    cv::imshow("First", edges);
    //fit circles
    HoughCircles(edges, tmpCircles, cv::HOUGH_GRADIENT, 1, 1, 25, 125, 40, 200);

    //add to existing circles
    circles->insert(circles->end(), tmpCircles.begin(), tmpCircles.end());

}

void visualize(cv::Vec3f *c, cv::Mat *frame, cv::Scalar col) {
    cv::Point center = cv::Point((*c)[0], (*c)[1]);

    circle(*frame, center, 1, cv::Scalar(0, 100, 100), 3, cv::LINE_AA);

    int radius = (*c)[2];
    circle(*frame, center, radius, col, 3, cv::LINE_AA);
}

void visualize(cv::Vec3f *c, cv::Mat *frame) {
    visualize(c, frame, cv::Scalar(255, 0, 255));
}

void visualizeVec(std::deque<cv::Vec3f> *circles, cv::Mat *frame) {
    for (size_t i = 0; i < circles->size(); i++) {
        cv::Vec3i c = (*circles)[i];
        visualize(&(*circles)[i], frame, cv::Scalar(0, 100, 100));
    }
}

void calcMeanCircle(std::deque<cv::Vec3f> *set, cv::Vec3f *circle) {
    double radius, x, y = 0.0;
    for (auto &tmpCircle: *set) {
        x += tmpCircle[0];
        y += tmpCircle[1];
        radius += tmpCircle[2];
    }
    x /= set->size();
    y /= set->size();
    radius /= set->size();

    (*circle)[0] = x;
    (*circle)[1] = y;
    (*circle)[2] = radius;
}

void calcMeanCircle(std::deque<cv::Vec3f> *set, int indexStart, int indexEnd, cv::Vec3f *circle) {
    double radius, x, y = 0.0;
    for (int i = indexStart; i < indexEnd; i++) {
        x += (*set)[i][0];
        y += (*set)[i][1];
        radius += (*set)[i][2];
    }
    x /= (indexEnd - indexStart);
    y /= (indexEnd - indexStart);
    radius /= (indexEnd - indexStart);

    (*circle)[0] = x;
    (*circle)[1] = y;
    (*circle)[2] = radius;
}

int maxDiff(std::vector<cv::Vec3f> *circles, cv::Vec3f *avgCircle1, cv::Vec3f *avgCircle2) {
    cv::Mat labels;
    std::vector<cv::Vec3f> centers;
    cv::kmeans(*circles, 2, labels, cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1.0), 3, cv::KMEANS_PP_CENTERS, centers);

    *avgCircle1 = centers.at(0);
    *avgCircle2 = centers.at(1);
}

//int maxDiff(std::deque<cv::Vec3f> *circles, cv::Vec3f *avgCircle1, cv::Vec3f *avgCircle2) {
//    //sort vector by radius
//    std::sort(circles->begin(), circles->end(), [](const cv::Vec3f &a, const cv::Vec3f &b) {
//        return a[2] > b[2];
//    });
//
//    if (circles->size() < 2) {
//        return 0;
//    }
//
//    double maxDiff = -1.0;
//    int maxDiffIndex = -1;
//    for (int i = 1; i < circles->size(); i++) {
//        double avgLower, avgHigher = 0.0;
//        for (int iLower = 0; iLower < i; iLower++) {
//            avgLower += (*circles)[iLower][2];
//        }
//        avgLower /= (i);
//        for (int iHigher = i; iHigher < circles->size(); iHigher++) {
//            avgHigher += (*circles)[iHigher][2];
//        }
//        avgHigher /= (circles->size() - i);
//
//        if (maxDiff < avgLower - avgHigher) {
//            maxDiff = avgLower - avgHigher;
//            maxDiffIndex = i;
//        }
//    }
//
//    calcMeanCircle(circles, 0, maxDiffIndex, avgCircle1);
//    calcMeanCircle(circles, maxDiffIndex, circles->size(), avgCircle2);
//    std::cout << "AVG - maxDiff: " << maxDiff << ", maxDiffIndex: " << maxDiffIndex << std::endl;
//}

int maxDiffOld(std::deque<cv::Vec3f> *circles, cv::Vec3f *avgCircle1, cv::Vec3f *avgCircle2) {
    //sort vector
    std::sort(circles->begin(), circles->end(), [](const cv::Vec3f &a, const cv::Vec3f &b) {
        return a[2] > b[2];
    });

    std::deque<cv::Vec3f> set1, set2;
    double sum1, sum2;
    cv::Vec3f front = circles->front();
    circles->pop_front();
    set1.push_front(front);
    sum1 = front[2];
    cv::Vec3f back = circles->back();
    circles->pop_back();
    set2.push_front(front);
    sum2 = back[2];

    while (!circles->empty()) {
        cv::Vec3f val = circles->front();
        circles->pop_front();
        if (abs(front[2] - val[2]) < abs(back[2] - val[2])) {
            set1.push_front(val);
            sum1 += val[2];
        } else {
            set2.push_front(val);
            sum2 += val[2];
        }
    }

    calcMeanCircle(&set1, avgCircle1);
    calcMeanCircle(&set2, avgCircle2);
}

int main(int, char**)
{
    //--- INITIALIZE VIDEOCAPTURE
    cv::VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usge: select any API backend

    int deviceID = 4;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID, apiID);
    while(!cap.isOpened()) {
        deviceID++;
        cap.open(deviceID, apiID);
        std::cout << deviceID << std::endl;
    }

    // check if we succeeded
    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    //--- GRAB AND WRITE LOOP
    std::cout << "Start grabbing" << std::endl
              << "Press any key to terminate" << std::endl;
    while(42) {
        std::vector<cv::Vec3f> circles;
        cv::Mat frame;

        while(circles.size() < 5) {
            captureFrame(&cap, &frame);
            //resize(frame, frame, cv::Size(), 0.15, 0.15);
            matchCircles(&circles, &frame);

            //if (cv::waitKey(5) >= 0)
            //    return 0;
            cv::waitKey(50);
            //visualizeVec(&circles, &frame);
            //cv::imshow("First", frame);
        }

        cv::Vec3f avgCircle1, avgCircle2;
        //visualizeVec(&circles, &frame);
        maxDiff(&circles, &avgCircle1, &avgCircle2);

        visualize(&avgCircle1, &frame);
        visualize(&avgCircle2, &frame);

        cv::imshow("Live", frame);

        if (cv::waitKey(50) >= 0)
            break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
