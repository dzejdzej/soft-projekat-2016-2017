// CannyTutorial.cpp : Defines the entry point for the console application.
// Environment: Visual studio 2015, Windows 10
// Assumptions: Opecv is installed configured in the visual studio project
// Opencv version: OpenCV 3.1

//#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include<string>
#include <stdio.h>
#include<iostream>
using namespace cv;



int mojCanny(Mat imgBlurred, Mat imgCanny, int, int, int)
{

        Canny(imgBlurred,            // input image
        imgCanny,                    // output image
        100,                        // low threshold
        200);                        // high threshold


    // Declare windows
    // Note: you can use CV_WINDOW_NORMAL which allows resizing the window
    // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
    // CV_WINDOW_AUTOSIZE is the default
    cv::namedWindow("imgCanny", CV_WINDOW_AUTOSIZE);

    //Show windows
    cv::imshow("imgCanny", imgCanny);

    cv::waitKey(0);                    // hold windows open until user presses a key
    return 0;
}
