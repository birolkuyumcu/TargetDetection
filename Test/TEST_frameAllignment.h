#ifndef TEST_FRAMEALLIGNMENT_H
#define TEST_FRAMEALLIGNMENT_H


#include <QString>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/video/background_segm.hpp"
#include "framealigner.h"
#include "alignmentmatrixcalc.h"
#include "framealignment.h"
#include "candidatedetector.h"
#include "candidatefilter.h"
#include "movevector.h"
#include <QFile>
#include <QTextStream>


typedef struct
{
    long    TotalTimeSn;
    float   fps;
    float   homograpyFoundPercent;
    double  whitePixelPerFramePixels;
    double  nCandidatePerFrame;
    QString HomographyMethod;
    QString usedDetector;
    QString usedDescriptor;
    QString videoFileName;
    int     neighbourhoodFilterSize;
}AllignementTestScore;

void TEST_frameAllignment();
long int timeMeasure(int i);
void TestforVideos(char * videoFileName);
double dynamicThresholdValue(cv::Mat &img, int k=5);
void DemoforVideos();
void moveVectorShow(char * videoFileName);
void writeRunParameters(char * pFileName);
void readRunParameters(char * pFileName);
void moveVectorClassShow(char * videoFileName);



#endif // TEST_FRAMEALLIGNMENT_H
