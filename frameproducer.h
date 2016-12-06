/*
 * Target Detection 
 * 
 * Copyright (C) Volkan Salma volkansalma@gmail.com
 * 				 Birol Kuyumcu  bluekid70@gmail.com
 * GPL v3 - https://github.com/birolkuyumcu/TargetDetection
 */
 
#ifndef FRAMEPRODUCER_H
#define FRAMEPRODUCER_H

#include <QThread>
#include <queue>
#include "exception.h"
#include <opencv2/opencv.hpp>

class FrameProducer : public QThread
{
private:
    Exception exc;
    double fps;
    QString             videoFileName;
    unsigned int        nBufferLimit;
    cv::Size            frameResolution;
    cv::VideoCapture    capture;
    Q_OBJECT
public:
    bool Stop;
    explicit FrameProducer(QObject *parent = 0);
    void openVideoFile(QString _videoFileName, cv::Size size_=cv::Size(320,240), int iBufferLimit=5);
    void run();
    std::queue<cv::Mat> frameBuffer;
    
signals:
    void framePushed();
    void readingEnd();
    
public slots:
    
};

#endif // FRAMEPRODUCER_H
