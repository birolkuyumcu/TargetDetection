#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QSemaphore>
#include "mainwindow.h"
#include <opencv2/opencv.hpp>
#include "preprocess.h"
#include "alignmentmatrixcalc.h"
#include "framealignment.h"


#define _CVS_IMG_BUFFER_SIZE 30

class ImageProcess : public QThread
{
    Q_OBJECT
public:

    Preprocess          preprocess;
    FrameAlignment      frameAligner;
    CandidateDetector   candidateDetector;
    CandidateFilter     candidateFilter;
    AlarmGenerator      alarmGenerator;
    AlignmentMatrixCalc alignmentCalc;

    explicit ImageProcess(QObject *parent = 0);
    void pushFrame(cv::Mat &inputImage);
    void connectGuiSlots(MainWindow &ui);
    void cloneImageWithIndex(unsigned int, cv::Mat &outputImg);

private:
    QTimer              timer1Hz;
    QSemaphore          semaphore;
    cv::Mat             imgBuffer[_CVS_IMG_BUFFER_SIZE];
    unsigned int        writeIndex;
    unsigned int        readIndex;
    unsigned int        processedFps;
    unsigned int        processedFrameCnt;
    void run();


    
public slots:
    void timerTick1Hz();

signals:
    void pushFrameToGui(void*);
    
};

#endif // IMAGEPROCESS_H
