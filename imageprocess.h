#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QSemaphore>
#include <opencv2/opencv.hpp>

#define _CVS_IMG_BUFFER_SIZE 30

class ImageProcess : public QThread
{
    Q_OBJECT
public:
    explicit ImageProcess(QObject *parent = 0);
    void pushFrame(cv::Mat &inputImage);

private:
    QTimer          timer1Hz;
    QSemaphore      semaphore;
    cv::Mat         imgBuffer[_CVS_IMG_BUFFER_SIZE];
    unsigned int    writeIndex;
    unsigned int    readIndex;
    unsigned int    processedFps;
    unsigned int    processedFrameCnt;

    void run();
    
public slots:
    void timerTick1Hz();
    
};

#endif // IMAGEPROCESS_H
