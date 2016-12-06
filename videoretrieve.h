/*
 * Target Detection 
 * 
 * Copyright (C) Volkan Salma volkansalma@gmail.com
 * 				 Birol Kuyumcu  bluekid70@gmail.com
 * GPL v3 - https://github.com/birolkuyumcu/TargetDetection
 */
 
#ifndef VIDEORETRIEVE_H
#define VIDEORETRIEVE_H

#include "imageretrieve.h"
#include <opencv2/opencv.hpp>

class VideoRetrieve : public ImageRetrieve
{
private:
    QString             videoFileName;
    unsigned int        currentFrameNumber;
    cv::Size            frameResolution;
    cv::VideoCapture    capture;
    cv::Mat             capturedFrame;
    cv::Mat             resizedFrame;
public:
    VideoRetrieve(ImageProcess* _pImgProcess);
    void openVideoFile(QString _videoFileName, unsigned int _frameNum, cv::Size size_);
    unsigned int getCurentFrameNum();
    void run();
};

#endif // VIDEORETRIEVE_H
