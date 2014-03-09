#include "frameproducer.h"

FrameProducer::FrameProducer(QObject *parent) :    QThread(parent)
{
        exc.setModuleName("FrameProducer");
}

void FrameProducer::openVideoFile(QString _videoFileName, std::queue<cv::Mat> *iframeBuffer, cv::Size size_, int iBufferLimit)
{
    if(capture.isOpened())
    {
        capture.release();
    }

    videoFileName = _videoFileName;
    frameResolution = _size;
    frameBuffer = iframeBuffer;
    nBufferLimit = iBufferLimit;


    capture.open(videoFileName.toStdString());

    if(!capture.isOpened())
    {
        exc.showException("Video acilamadi.");
    }
    else
    {
        fps = capture.get(CV_CAP_PROP_FPS);
    }


}

void FrameProducer::run()
{
    cv::Mat capturedFrame;

    while(1)
    {
        if(frameBuffer->size() < nBufferLimit )
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
                    cv::resize(capturedFrame, capturedFrame, frameResolution);

                }

                frameBuffer->push(capturedFrame);
            }

            QThread::msleep(1000./fps);
        }
    }
}
