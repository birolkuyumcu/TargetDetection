#ifndef FRAMECONSUMER_H
#define FRAMECONSUMER_H

#include <QThread>
#include <queue>
#include "exception.h"
#include "framealigner.h"
#include "alignmentmatrixcalc.h"
#include "framealignment.h"
#include "candidatedetector.h"
#include "candidatefilter.h"

class FrameConsumer : public QThread
{
private:
    Exception exc;
    std::queue<cv::Mat> *frameBuffer;
    AlignmentMatrixCalc calc;
    FrameAlignment aligner;
    CandidateDetector cDet;
    CandidateFilter cFilt;
    cv::Mat pFrame;
    int nPass;


    Q_OBJECT
public:
    explicit FrameConsumer(QObject *parent = 0);
    void run();
    void setBuffers(std::queue<cv::Mat> *iframeBuffer);
    void setParameters(QVector<QString> &parameterTexts );
    bool isReadingEnd;

    cv::Mat processedFrame2UiTargets;
    cv::Mat processedFrame2UiAbsDiff;
    cv::Mat processedFrame2UiCandidates;

    
signals:
    void frameProcessed();
    void processingEnd();    
    
public slots:
    
};

#endif // FRAMECONSUMER_H
