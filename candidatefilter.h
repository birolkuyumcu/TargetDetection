#ifndef CANDIDATEFILTER_H
#define CANDIDATEFILTER_H

#include <opencv2/opencv.hpp>
#include "exception.h"

enum TargetState
{
    candidate,
    visible,
    invisible
};

class CandidateFilterSettings
{
public:
    int dummy;
};

class Target
{
public:
    cv::RotatedRect location;
    TargetState status;
    unsigned statusCounter;

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
    void process(std::vector<cv::RotatedRect> *iCandidateList);
    void init();
    void match();
    //
    std::vector<Target> targetList;
    std::vector<cv::RotatedRect> *candidateList;

private:
    CandidateFilterSettings settings;
    Exception               exc;
    float calculateDistance(cv::RotatedRect& r1,cv::RotatedRect& r2);
};

#endif // CANDIDATEFILTER_H
