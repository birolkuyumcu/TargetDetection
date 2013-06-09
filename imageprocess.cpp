#include "imageprocess.h"

ImageProcess::ImageProcess(QObject *parent) :
    QThread(parent)
{
    writeIndex = 0;
    readIndex = 1;

    connect(&timer1Hz, SIGNAL(timeout()), this, SLOT(timerTick1Hz()));

    timer1Hz.setInterval(1000);
    timer1Hz.start();
}

void ImageProcess::timerTick1Hz()
{
    processedFps = processedFrameCnt;
    processedFrameCnt = 0;

    qDebug("fps:%d", processedFps);

}

void ImageProcess::pushFrame(cv::Mat &img)
{

    writeIndex ++;
    if(writeIndex >= _CVS_IMG_BUFFER_SIZE)
    {
        writeIndex = 0;
    }

    img.copyTo(imgBuffer[writeIndex]);


    semaphore.release();

}

void ImageProcess::run()
{
    while(1)
    {
        semaphore.acquire(); //wait for semaphore

        cv::imshow("Testing", imgBuffer[readIndex]);

        processedFrameCnt ++;

        readIndex ++;

        if(readIndex >= _CVS_IMG_BUFFER_SIZE)
        {
            readIndex = 0;
        }

    }
}


