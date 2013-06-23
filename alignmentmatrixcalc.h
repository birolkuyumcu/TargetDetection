#ifndef ALIGNMENTMATRIXCALC_H
#define ALIGNMENTMATRIXCALC_H

#include <opencv2/opencv.hpp>
#include "exception.h"

class AlignmentMatrixCalcSettings
{
public:
    int dummy;
};

class AlignmentMatrixCalc
{
public:
    AlignmentMatrixCalc();
    void set(AlignmentMatrixCalcSettings& _settings);
    void getSettings(AlignmentMatrixCalcSettings& _settings);
    void saveSettings();
    bool loadSettings();
    void process(cv::Mat& inputImage);

private:
    AlignmentMatrixCalcSettings settings;
    Exception                   exc;

};

#endif // ALIGNMENTMATRIXCALC_H
