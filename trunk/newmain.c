/* 
 * File:   newmain.cpp
 * Author: gustavo
 *
 * Created on August 20, 2013, 8:40 AM
 */

#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>

IplImage temp;
using namespace std;

int main() {
    cv::Mat frame;
    cv::Mat back;
    cv::Mat fore;
    cv::VideoCapture cap(0);
    cv::BackgroundSubtractorMOG2 bg;
    cv::Point min, mid, max;
    vector<vector<cv::Point> > contours;
    int i, j;

    bg.nmixtures = 3; // set number of gaussian mixtures
    bg.bShadowDetection = false; // turn the shadow detection off    
    //    bg.history = 10;

    cv::namedWindow("Frame");
    cv::namedWindow("Background");

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

    for (;;) {
        cap >> frame;
        bg.operator ()(frame, fore);
        bg.getBackgroundImage(back);
        cv::erode(fore, fore, cv::Mat());
        cv::dilate(fore, fore, cv::Mat());
        cv::findContours(fore, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        cv::drawContours(frame, contours, -1, cv::Scalar(0, 0, 255), 2);

        max = (contours.front()).front();
        min = max;
        for (i = 0; i < contours.size(); i++) {
            for (j = 0; j < (contours.at(i)).size(); j++) {
                if ((contours.at(i)).at(j).x < min.x)
                    min.x = (contours.at(i)).at(j).x;
                if ((contours.at(i)).at(j).y < min.y)
                    min.y = (contours.at(i)).at(j).y;
                if ((contours.at(i)).at(j).x > max.x)
                    max.x = (contours.at(i)).at(j).x;
                if ((contours.at(i)).at(j).y > max.y)
                    max.y = (contours.at(i)).at(j).y;
            }
        }

        mid.x = (min.x + max.x) / 2.0;
        mid.y = (min.y + max.y) / 2.0;
        cout << "X: " << mid.x << " Y: " << mid.y << endl;

        cv::circle(frame, mid, 5, 10);

        cv::imshow("Frame", frame);
        cv::imshow("Background", back);

        if (cv::waitKey(30) >= 0) {
            cv::imwrite("image.png", frame, compression_params);

            temp = frame;
            cvSaveImage("image.jpg", &temp);

            break;
        }
    }
    
    return 0;
}
