#ifndef CANDIDATEDETECTOR_H
#define CANDIDATEDETECTOR_H

#include <opencv2/opencv.hpp>
#include "exception.h"

class CandidateDetectorSettings
{
public:
    int dummy;
};

class CandidateDetector
{
public:
    CandidateDetector();
    void set(CandidateDetectorSettings& _settings);
    void getSettings(CandidateDetectorSettings& _settings);
    void saveSettings();
    bool loadSettings();
    void process(cv::Mat& inputImage);

private:
    CandidateDetectorSettings   settings;
    Exception                   exc;
};

#endif // CANDIDATEDETECTOR_H
