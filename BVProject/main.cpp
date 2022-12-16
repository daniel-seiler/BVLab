#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv2/imgproc.hpp>

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

    //fit circles
    HoughCircles(preparedFrame, tmpCircles, cv::HOUGH_GRADIENT, 1, 15, 25, 90, 40, 200);

    //add to existing circles
    circles->insert(circles->end(), tmpCircles.begin(), tmpCircles.end());

}

void visualize(cv::Vec3f *c, cv::Mat *frame) {
    cv::Point center = cv::Point((*c)[0], (*c)[1]);

    circle(*frame, center, 1, cv::Scalar(0, 100, 100), 3, cv::LINE_AA);

    int radius = (*c)[2];
    circle(*frame, center, radius, cv::Scalar(255, 0, 255), 3, cv::LINE_AA);
}

void visualizeVec(std::vector<cv::Vec3f> *circles, cv::Mat *frame) {
    for (size_t i = 0; i < circles->size(); i++) {
        cv::Vec3i c = (*circles)[i];
        visualize(&(*circles)[i], frame);
    }

    cv::imshow("Live", *frame);
}

void calcMeanCircle(std::vector<cv::Vec3f> *set, cv::Vec3f *circle) {
    double radius, x, y = 0.0;
    for (auto &tmpCircle: *set) {
        x += tmpCircle[0];
        y += tmpCircle[1];
        radius += tmpCircle[2];
    }
    x /= set->size();
    y /= set->size();
    radius /= set->size();

    circle[0] = x;
    circle[1] = y;
    circle[2] = radius;
    std::cout << radius << std::endl;
}

bool compareCircles(const cv::Vec3f &a, const cv::Vec3f &b) {
    return a[2] < b[2];
}

int maxDiff(std::vector<cv::Vec3f> *circles) {
    int bestIndex = -1;
    double maxDiff = 0.0;
    for (int i = 0; i < circles->size(); i++) {
        std::vector<cv::Vec3f> set1(circles->begin(), circles->begin() + i);
        std::vector<cv::Vec3f> set2(circles->begin() + i, circles->end());
        cv::Vec3f tmpC1;
        cv::Vec3f tmpC2;
        calcMeanCircle(&set1, &tmpC1);
        calcMeanCircle(&set2, &tmpC2);
        if (tmpC1[2] > 0 && tmpC1[2] < 300 && tmpC2[2] > 0 && tmpC2[2] < 300) {
            double tmpDiff = tmpC1[2] - tmpC2[2];
            if (tmpDiff > maxDiff) {
                maxDiff = tmpDiff;
                bestIndex = i;
            }
        }
    }
    return bestIndex;
}

int main(int, char**)
{
    //--- INITIALIZE VIDEOCAPTURE
    cv::VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID, apiID);
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

        //TODO replace for with while(numberOfCircles)
        for(int i = 0; i < 5; i++) {
            captureFrame(&cap, &frame);
            matchCircles(&circles, &frame);

            //sort vector
            std::sort(circles.begin(), circles.end(), compareCircles);

            if (cv::waitKey(5) >= 0)
                return 0;
        }

        std::vector<cv::Vec3f> set1(circles.size());
        std::vector<cv::Vec3f> set2(circles.size());
        int splitIndex = maxDiff(&circles);
        cv::Vec3f c1, c2;
        //calcMeanCircle(&set1, &c1);
        //calcMeanCircle(&set2, &c2);

        //visualize(&c1, &frame);
        //visualize(&c2, &frame);

        std::cout << std::endl << std::endl;
        for(int i = 0; i < circles.size(); i++) {
            std::cout << circles[i][2] << std::endl;
        }
        std::cout << "Split index: " << splitIndex << std::endl << std::endl;

        visualizeVec(&circles, &frame);
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
