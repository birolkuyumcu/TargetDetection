#ifndef TEST_FRAMEALLIGNMENT_H
#define TEST_FRAMEALLIGNMENT_H


#include <QString>
#include <opencv2/opencv.hpp>
#include "framealigner.h"
#include "alignmentmatrixcalc.h"
#include "framealignment.h"
#include <QFile>
#include <QTextStream>


typedef struct
{
    long    TotalTimeSn;
    float   fps;
    float   homograpyFoundPercent;
    float   whitePixelPerFramePixels;
    QString HomographyMethod;
    QString usedDetector;
    QString usedDescriptor;
    QString videoFileName;
    int     neighbourhoodFilterSize;
}AllignementTestScore;

void TEST_frameAllignment();
long int timeMeasure(int i);




#endif // TEST_FRAMEALLIGNMENT_H
