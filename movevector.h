/*
 * Target Detection 
 * 
 * Copyright (C) Volkan Salma volkansalma@gmail.com
 * 				 Birol Kuyumcu  bluekid70@gmail.com
 * GPL v3 - https://github.com/birolkuyumcu/TargetDetection
 */
 
#ifndef MOVEVECTOR_H
#define MOVEVECTOR_H

#include "opencv2/nonfree/nonfree.hpp"
#include <opencv2/opencv.hpp>
#include "exception.h"

class MoveVector
{
public:
    MoveVector();
    void process(cv::Mat& inputImage);
    void show();

    // Calculated Vector values for X an Y axes
    double mDeltaX;
    double mDeltaY;

private:
    Exception exc;
    cv::Mat currentFrame;
    cv::Mat prevFrame;
    std::vector<cv::Point2f> pointsPrev;
    int stepX;
    int stepY;

    int centerX;
    int centerY;


    void init();
    void calc();
};

#endif // MOVEVECTOR_H
