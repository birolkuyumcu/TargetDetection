#include "frameconsumer.h"

FrameConsumer::FrameConsumer(QObject *parent) :
    QThread(parent)
{
            exc.setModuleName("FrameConsumer");
}

void FrameConsumer::run()
{
    cv::Mat prevFrame;
    cv::Mat alignedPrevFrame;
    cv::Mat currentDiffImage;

    while(1)
    {
        //       qDebug()<<frameBuffer->size()<<"Consumer Side\n";
        if(frameBuffer->size() > 0 )
        {
            cv::Mat frame = frameBuffer->front();
            //Do Processing
            pFrame = frame.clone();
            cv::cvtColor(pFrame,pFrame,CV_RGB2GRAY);

            cv::Mat copyCurrentFrame=pFrame.clone();
            cv::Mat H;
            calc.process(copyCurrentFrame);

            if(calc.getHomography(H) == true)
            {
                qDebug()<<"Homography Found \n";
                // Düzgün dönüşüm matrisi bulunduysa
                cv::Mat mask(prevFrame.size(),CV_8U);
                mask=cv::Scalar(255);

                // Önceki frame aktif frame çevir
                aligner.process(prevFrame,H,alignedPrevFrame);
                // çevrilmiş önceki frame için maske oluştur
                aligner.process(mask,H,mask);
                mask=copyCurrentFrame&mask;
                cv::absdiff(alignedPrevFrame,mask,currentDiffImage);
                processedFrameBuffer->push(currentDiffImage);
                emit frameProcessed();

            }
            prevFrame=copyCurrentFrame;



            // End Processing

      //      QThread::msleep(1000./25);
            frameBuffer->pop();
        }
    }
}

void FrameConsumer::setBuffers(std::queue<cv::Mat> *iframeBuffer, std::queue<cv::Mat> *iprocessedFrameBuffer)
{
        frameBuffer = iframeBuffer;
        processedFrameBuffer = iprocessedFrameBuffer;
}
