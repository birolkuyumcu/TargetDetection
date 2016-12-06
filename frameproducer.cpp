/*
 * Target Detection 
 * 
 * Copyright (C) Volkan Salma volkansalma@gmail.com
 * 				 Birol Kuyumcu  bluekid70@gmail.com
 * GPL v3 - https://github.com/birolkuyumcu/TargetDetection
 */
 
#include "frameproducer.h"

FrameProducer::FrameProducer(QObject *parent) :    QThread(parent)
{
        exc.setModuleName("FrameProducer");

}

void FrameProducer::openVideoFile(QString _videoFileName, cv::Size _size, int iBufferLimit)
{
    if(capture.isOpened())
    {
        capture.release();
    }

    videoFileName = _videoFileName;
    frameResolution = _size;
    nBufferLimit = iBufferLimit;


    capture.open(videoFileName.toStdString());

    if(!capture.isOpened())
    {
        exc.showException("Video acilamadi.");
    }
    else
    {
        fps = 10;//capture.get(CV_CAP_PROP_FPS);
    }


}

void FrameProducer::run()
{
    cv::Mat capturedFrame;

    while(1)
    {
        if(frameBuffer.size() < nBufferLimit )
        {

            capture >> capturedFrame;

            if(!capturedFrame.data)
            {
                exc.showException("Frame alinamadi..");
                break;
            }
            else
            {
               /* if(capturedFrame.size() != frameResolution)
                {
                    cv::resize(capturedFrame, capturedFrame, frameResolution);

                }
                */
                frameBuffer.push(capturedFrame);
            }

            emit framePushed();
        }

       QThread::msleep(1000./fps);
    }
}

