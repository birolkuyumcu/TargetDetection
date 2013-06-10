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

void ImageProcess::pushFrame(cv::Mat &inputImage)
{

    writeIndex ++;
    if(writeIndex >= _CVS_IMG_BUFFER_SIZE)
    {
        writeIndex = 0;
    }

    inputImage.copyTo(imgBuffer[writeIndex]);


    semaphore.release();

}

void ImageProcess::run()
{
    while(1)
    {
        semaphore.acquire(); //wait for semaphore

        //image processing will be done in here
        //use imgBuffer[readIndex] as input image

        /*.
        .
        .
        .

        .*/


        //push result to view
        emit pushFrameToGui((void*)&imgBuffer[readIndex]);


        readIndex ++;
        if(readIndex >= _CVS_IMG_BUFFER_SIZE)
        {
            readIndex = 0;
        }


        processedFrameCnt ++;
    }
}

void ImageProcess::cloneImageWithIndex(unsigned int index, cv::Mat &outputImg)
{
    outputImg = imgBuffer[index].clone();
}

void ImageProcess::connectGuiSlots(MainWindow &ui)
{
    QObject::connect(this, SIGNAL(pushFrameToGui(void*)), &ui, SLOT(refreshImgProcessingImg(void*)));
}



