/*
 * Target Detection 
 * 
 * Copyright (C) Volkan Salma volkansalma@gmail.com
 * 				 Birol Kuyumcu  bluekid70@gmail.com
 * GPL v3 - https://github.com/birolkuyumcu/TargetDetection
 */
 
#ifndef CANDIDATEFILTER_H
#define CANDIDATEFILTER_H

#include <opencv2/opencv.hpp>
#include "exception.h"
#include "candidatedetector.h"
#include <vector>
#include <list>

/* Target states
 * candidate ; initial stage
 * visible   ; candidate becomes visible after statuscounter reached visibility theshold
 * invisible ; visible becomes  invsible if unmatched
*/
enum TargetState
{
    candidate,
    visible,
    invisible
};


class CandidateFilterSettings
{
public:

    int visibilityThreshold;  // status from candidate to visible
    int invisibilityThreshold; // status from invisible to remove
    float distanceThreshold; // minmatching distance
    // showTargets settings
    bool showCandidate;
    bool showVisible;
    bool showInvisible;
    bool showTargetId;
    bool showVector;
};

class Target
{
public:
    cv::RotatedRect location;
    std::vector<cv::Point> contour;
    TargetState status;
    unsigned statusCounter;
    bool isMatched;
    unsigned targetId;
    // vector of Move
    float matchingDistance;
    float movingAngle;
    float deltaX;
    float deltaY;
    //
    bool isWithin(Target& isSubTarget);
    void setMovingAngle(cv::RotatedRect r);



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
    void process(std::vector<Candidate> *iCandidateList);
    void processUnmatchedTargets();
    void processUnmatchedCandidates();
    void init();
    void match();
    void showTargets(cv::Mat& inputImage,float scaleFactor=1 , char *wName=NULL);
    void refine();

    //
    std::vector<Target> targetList;
    std::vector<Candidate> *candidateList;
    std::vector<bool> isCandidateMatched;
    CandidateFilterSettings settings;

private:

    Exception               exc;
    unsigned targetIdCounter;
    float calculateDistance(cv::RotatedRect& r1,cv::RotatedRect& r2);
    bool isNewTarget(Target &tempTarget);

};

#endif // CANDIDATEFILTER_H
