#include "videoretrieve.h"
#include <QThread>

VideoRetrieve::VideoRetrieve(ImageProcess* _pImgProcess)
{
    pImgProcess = _pImgProcess;
    exc.setModuleName("VideoRetrieve");
}

void VideoRetrieve::run()
{
    while(1)
    {

        capture >> capturedFrame;

        if(!capturedFrame.data)
        {
            exc.showException("Frame alinamadi..");
        }
        else
        {
            if(capturedFrame.size() != frameResolution)
            {
                cv::resize(capturedFrame, resizedFrame, frameResolution);
                pImgProcess->pushFrame(resizedFrame);
            }
            else
            {
                pImgProcess->pushFrame(capturedFrame);
            }
        }

        QThread::msleep(1000./fps);
    }
}

void VideoRetrieve::openVideoFile(QString _videoFileName, unsigned int _frameNum, cv::Size _size)
{
    if(capture.isOpened())
    {
        capture.release();
    }

    videoFileName = _videoFileName;
    currentFrameNumber = _frameNum;
    frameResolution = _size;

    capture.set(CV_CAP_PROP_POS_FRAMES, (double)currentFrameNumber);
    capture.open(videoFileName.toStdString());

    if(! capture.isOpened())
    {
        exc.showException("Video acilamadi.");
    }
    else
    {
        resizedFrame = cvCreateImage(frameResolution, CV_8UC3, 3);
    }

}

unsigned int VideoRetrieve::getCurentFrameNum()
{
    return currentFrameNumber;
}
