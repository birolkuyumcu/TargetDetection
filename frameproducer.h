#ifndef FRAMEPRODUCER_H
#define FRAMEPRODUCER_H

#include <QThread>
#include <queue>
#include "exception.h"

class FrameProducer : public QThread
{
private:
    Exception exc;
    double fps;
    QString             videoFileName;
    unsigned int        nBufferLimit;
    cv::Size            frameResolution;
    cv::VideoCapture    capture;
    std::queue<cv::Mat> *frameBuffer;

    Q_OBJECT
public:
    explicit FrameProducer(QObject *parent = 0);
    void openVideoFile(QString _videoFileName, std::queue<cv::Mat> *iframeBuffer , cv::Size size_=cv::Size(320,240), int iBufferLimit=15);
    void run();
    
signals:
    
public slots:
    
};

#endif // FRAMEPRODUCER_H
