#ifndef TEST_FRAMEALLIGNMENT_H
#define TEST_FRAMEALLIGNMENT_H


#include <QString>
#include <opencv2/opencv.hpp>
#include "framealigner.h"
#include "alignmentmatrixcalc.h"
#include "framealignment.h"
#include "candidatedetector.h"
#include "candidatefilter.h"
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




#endif // TEST_FRAMEALLIGNMENT_H
