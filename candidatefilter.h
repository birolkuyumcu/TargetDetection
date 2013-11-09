#ifndef CANDIDATEFILTER_H
#define CANDIDATEFILTER_H

#include <opencv2/opencv.hpp>
#include "exception.h"
#include <vector>
#include <list>

enum TargetState
{
    candidate,
    visible,
    invisible
};

class CandidateFilterSettings
{
public:
    int visibilityThreshold;
    int invisibilityThreshold;
    float distanceThreshold;
};

class Target
{
public:
    cv::RotatedRect location;
    TargetState status;
    unsigned statusCounter;
    bool isMatched;
    unsigned targetId;

};

class MatchItem
{
public:
    int candidateIndex;
    int targetIndex;
    float distance;
    bool operator<(const MatchItem &other) const { return distance < other.distance; }
};

class CandidateFilter
{
public:
    CandidateFilter();
    void set(CandidateFilterSettings& _settings);
    void getSettings(CandidateFilterSettings& _settings);
    void saveSettings();
    bool loadSettings();
    void process(std::vector<cv::RotatedRect> *iCandidateList);
    void processUnmatchedTargets();
    void processUnmatchedCandidates();
    void init();
    void match();
    void showTargets(cv::Mat& inputImage,char *wName=NULL);
    //
    std::vector<Target> targetList;
    std::vector<cv::RotatedRect> *candidateList;
    std::vector<bool> isCandidateMatched;

private:
    CandidateFilterSettings settings;
    Exception               exc;
    unsigned targetIdCounter;
    float calculateDistance(cv::RotatedRect& r1,cv::RotatedRect& r2);
};

#endif // CANDIDATEFILTER_H
