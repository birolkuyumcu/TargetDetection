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
    int     fps;
    float   homograpyFoundPercent;
    float   whitePixelPerFrame;
    QString HomographyMethod;
    QString usedDetector;
    QString usedDescriptor;

}AllignementTestScore;

void TEST_frameAllignment();
long int timeMeasure(int i);




#endif // TEST_FRAMEALLIGNMENT_H
