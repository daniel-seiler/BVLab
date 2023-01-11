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

void visualize(std::deque<cv::Vec3f> *circles, cv::Mat *frame) {
    for (size_t i = 0; i < circles->size(); i++) {
        cv::Vec3i c = (*circles)[i];
        visualize(&(*circles)[i], frame, cv::Scalar(0, 100, 100));
    }
}

int cluster(std::vector<cv::Vec3f> *circles, cv::Vec3f *avgCircle1, cv::Vec3f *avgCircle2) {
    cv::Mat labels;
    std::vector<cv::Vec3f> centers;
    cv::kmeans(*circles, 2, labels, cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1.0), 3, cv::KMEANS_PP_CENTERS, centers);

    *avgCircle1 = centers.at(0);
    *avgCircle2 = centers.at(1);
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
        //visualize(&circles, &frame);
        cluster(&circles, &avgCircle1, &avgCircle2);

        visualize(&avgCircle1, &frame);
        visualize(&avgCircle2, &frame);

        cv::imshow("Live", frame);

        if (cv::waitKey(50) >= 0)
            break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
