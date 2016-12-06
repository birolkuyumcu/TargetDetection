/*
 * Target Detection 
 * 
 * Copyright (C) Volkan Salma volkansalma@gmail.com
 * 				 Birol Kuyumcu  bluekid70@gmail.com
 * GPL v3 - https://github.com/birolkuyumcu/TargetDetection
 */
 
#ifndef CANDIDATEDETECTOR_H
#define CANDIDATEDETECTOR_H

#include <opencv2/opencv.hpp>
#include "exception.h"

class CandidateDetectorSettings
{
public:
    int dummy;
    //dimentional upper and lowaer limits
    int minWidth;
    int maxWidth;
    int minHeight;
    int maxHeight;
};

class Candidate
{
    public:
    std::vector<cv::Point> contour;
    cv::RotatedRect rRect;

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
    void showCandidates(cv::Mat& inputImage,char* wName=NULL);
    float scaleFactor;

    //
  //  std::vector<std::vector<cv::Point> > candidateList;
  //  std::vector<cv::RotatedRect> candidateRRectsList;
    std::vector<Candidate> candidateList;

private:
    CandidateDetectorSettings   settings;
    Exception                   exc;
    int iWidth ;
    int iHeight;
};

#endif // CANDIDATEDETECTOR_H
