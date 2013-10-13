#ifndef CANDIDATEDETECTOR_H
#define CANDIDATEDETECTOR_H

#include <opencv2/opencv.hpp>
#include "exception.h"

class CandidateDetectorSettings
{
public:
    int dummy;
    int minWidth;
    int maxWidth;
    int minHeight;
    int maxHeight;
};

class CandidateDetector
{
public:
    CandidateDetector();
    void set(CandidateDetectorSettings& _settings);
    void getSettings(CandidateDetectorSettings& _settings);
    void saveSettings();
    bool loadSettings();
    void process(cv::Mat inputImage);

    //
    std::vector<std::vector<cv::Point> > candidateList;
    std::vector<cv::RotatedRect> candidateRRectsList;

private:
    CandidateDetectorSettings   settings;
    Exception                   exc;
};

#endif // CANDIDATEDETECTOR_H
