#include "imageprocess.h"

ImageProcess::ImageProcess(QObject *parent) :
    QThread(parent)
{
    writeIndex = 0;
    readIndex = 1;

    connect(&timer1Hz, SIGNAL(timeout()), this, SLOT(timerTick1Hz()));

    timer1Hz.setInterval(1000);
    timer1Hz.start();
    //


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
    cv::Mat pFrame;
    cv::Mat frame;

    while(1)
    {
        cv::Mat aPrev;
        cv::Mat H;
        semaphore.acquire(); //wait for semaphore

        //image processing will be done in here
        //use imgBuffer[readIndex] as input image

        /*.
        .
        .
        .

        .*/



        if(readIndex%10 == 0)// frame atlama
        {
            frame = imgBuffer[readIndex].clone();
            preprocess.process(frame);
            alignmentCalc.process(frame);
            pFrame = frame;
            if(alignmentCalc.getHomography(H))
            {
                frameAligner.process(pFrame,H,aPrev);
                cv::absdiff(aPrev,frame,aPrev);
                cv::threshold(aPrev,aPrev,0,255,cv::THRESH_BINARY|cv::THRESH_OTSU);
                cv::imshow("Result",aPrev);
                cv::waitKey(5);


            }
        }
        emit pushFrameToGui((void*)&imgBuffer[readIndex]);



        //push result to view
 //       emit pushFrameToGui((void*)&imgBuffer[readIndex]);


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



