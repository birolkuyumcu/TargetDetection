#ifndef CANDIDATEFILTER_H
#define CANDIDATEFILTER_H

#include <opencv2/opencv.hpp>
#include "exception.h"

class CandidateFilterSettings
{
public:
    int dummy;
};

class CandidateFilter
{
public:
    CandidateFilter();
    void set(CandidateFilterSettings& _settings);
    void getSettings(CandidateFilterSettings& _settings);
    void saveSettings();
    bool loadSettings();
    void process(cv::Mat& inputImage);

private:
    CandidateFilterSettings settings;
    Exception               exc;
};

#endif // CANDIDATEFILTER_H
