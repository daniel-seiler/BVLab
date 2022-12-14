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

void visualize(std::vector<cv::Vec3f> *circles, cv::Mat *frame) {
  for (size_t i = 0; i < circles->size(); i++) {
    cv::Vec3i c = (*circles)[i];
    cv::Point center = cv::Point(c[0], c[1]);

    circle(*frame, center, 1, cv::Scalar(0, 100, 100), 3, cv::LINE_AA);

    int radius = c[2];
    circle(*frame, center, radius, cv::Scalar(255, 0, 255), 3, cv::LINE_AA);
    //std::cout << radius << std::endl;
  }

  cv::imshow("Live", *frame);
}

bool compareCircles(const cv::Vec3f &a, const cv::Vec3f &b) {
  return a[2] < b[2];
}

void maxDiff(std::vector<cv::Vec3f> *circles, std::vector<cv::Vec3f> *set1, std::vector<cv::Vec3f> *set2) {
  //TODO implement equivalent of calcThreshByOtsu to divide vector
}

int main(int, char**)
{
  //--- INITIALIZE VIDEOCAPTURE
  cv::VideoCapture cap;
  // open the default camera using default API
  // cap.open(0);
  // OR advance usage: select any API backend
  int deviceID = 4;             // 0 = open default camera
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

    std::cout << std::endl << std::endl;
    for(int i = 0; i < circles.size(); i++) {
      std::cout << circles[i][2] << std::endl;
    }
    std::cout << std::endl << std::endl;

    visualize(&circles, &frame);
  }
  // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}
