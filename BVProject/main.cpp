#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv2/imgproc.hpp>
#include <deque>

#define LINE_THICKNESS 1
#define R_EMPTY 62      //in px
#define R_FULL 80.8     //in px
#define R_CUP_TOP 97.4  //in px
#define HIGHT_CAM 270   //in mm
#define HIGHT_CUP 90    //in mm
#define R_TOP 50        //in mm

void captureFrame(cv::VideoCapture* cap, cv::Mat *frame) {
    cap->read(*frame);
    if (frame->empty()) {
        std::cerr << "ERROR! blank frame grabbed" << std::endl;
    }
}

float avgVector(std::vector<float> *v) {
    float avg = 0;
    for (auto &tmp : *v) {
        avg += tmp;
    }
    return avg / v->size();
}

void visualize(cv::Vec3f *c, cv::Mat *frame, cv::Scalar col) {
    cv::Point center = cv::Point((*c)[0], (*c)[1]);

    circle(*frame, center, 1, cv::Scalar(0, 100, 100), 3, cv::LINE_AA);

    int radius = (*c)[2];
    circle(*frame, center, radius, col, 1, cv::LINE_AA);
}

void visualizeVec(std::vector<cv::Vec3f> *circles, cv::Mat *frame) {
    for (size_t i = 0; i < circles->size(); i++) {
        cv::Vec3i c = (*circles)[i];
        visualize(&(*circles)[i], frame, cv::Scalar(255, 0, 255));
    }
}

void matchCircles(std::vector<cv::Vec3f> *circles, cv::Mat *frame) {
    cv::Mat preparedFrame;
    std::vector<cv::Vec3f> tmpCircles;

    //prepare frame
    cv::cvtColor(*frame, preparedFrame, 6);
    cv::medianBlur(preparedFrame, preparedFrame, 5);
    cv::imshow("Blur", preparedFrame.clone());
    cv::Mat edges;
    Canny(preparedFrame, edges, 100, 200, 5, false);
    cv::imshow("Canny", edges);
    //fit circles
    HoughCircles(edges, tmpCircles, cv::HOUGH_GRADIENT, 1, 1, 25, 125, 40, 200);

    //add to existing circles
    circles->insert(circles->end(), tmpCircles.begin(), tmpCircles.end());
    visualizeVec(circles, &edges);
    cv::imshow("Hough", edges);
}

void visualize(cv::Vec3f *c, cv::Mat *frame) {
    visualize(c, frame, cv::Scalar(255, 0, 255));
}

int cluster(std::vector<cv::Vec3f> *circles, cv::Vec3f *avgCircle1, cv::Vec3f *avgCircle2) {
    cv::Mat labels;
    std::vector<cv::Vec3f> centers;
    cv::kmeans(*circles, 2, labels, cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1.0), 3, cv::KMEANS_PP_CENTERS, centers);

    *avgCircle1 = centers.at(0);
    *avgCircle2 = centers.at(1);
}

void calibrate(cv::Vec3f *c1, cv::Vec3f *c2, std::vector<float> *m) {
    //m->push_back(abs((*c2)[2] - (*c1)[2]));
    m->push_back(std::min((*c2)[2], (*c1)[2]));
}

float calcVolume(cv::Vec3f *c1, cv::Vec3f *c2) {
    float r_max = std::max((*c1)[2], (*c2)[2]);
    float r_coffee = std::min((*c1)[2], (*c2)[2]);
    std::cout << "small: " << r_coffee << " | big: " << r_max << std::endl;
    return 1 - ( (r_max - r_coffee)  / (r_max - R_EMPTY) );
}

int main(int, char**)
{
    //--- INITIALIZE VIDEOCAPTURE
    cv::VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usge: select any API backend
    int deviceID = 1;             // 0 = open default camera
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

    std::vector<float> measurements;
    while(42) {
        std::vector<cv::Vec3f> circles;
        cv::Mat frame;

        while(circles.size() < 20) {
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
        //maxDiff(&circles, &avgCircle1, &avgCircle2);

        visualize(&avgCircle1, &frame);
        visualize(&avgCircle2, &frame);

        calibrate(&avgCircle1, &avgCircle2, &measurements);
        std::cout << "Volume percent: " << calcVolume(&avgCircle1, &avgCircle2) * 100 << "%" << std::endl;

        cv::imshow("Live", frame);

        if (cv::waitKey(50) >= 0)
            break;
    }

    std::cout << "Average measurement: " << avgVector(&measurements) << " amount values: " << measurements.size() << std::endl;
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
