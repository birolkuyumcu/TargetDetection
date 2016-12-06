/*
 * Target Detection 
 * 
 * Copyright (C) Volkan Salma volkansalma@gmail.com
 * 				 Birol Kuyumcu  bluekid70@gmail.com
 * GPL v3 - https://github.com/birolkuyumcu/TargetDetection
 */
 
#ifndef FRAMEALIGNER_H
#define FRAMEALIGNER_H

#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/video/tracking.hpp"
#include "exception.h"
#include "alignmentmatrixcalc.h"

//enum HomograpyMethod {featureBased,flowBased};

class frameAligner
{
protected:
    Exception    exc;
private:
    cv::Mat currentFrame;
    cv::Mat prevFrame;
    cv::Mat descriptorsCurrent;
    cv::Mat descriptorsPrev;
    cv::Mat homography;
    int homographyCalcMethod;
    double ransacReprojThreshold;
    float flowErrorThreshold;
    unsigned int minimumFlowPoint;
    HomograpyMethod hMethod;
    cv::Ptr<cv::FeatureDetector> detector;
    cv::Ptr<cv::DescriptorExtractor> descriptor;
    cv::Ptr<cv::DescriptorMatcher> matcher;
    std::vector<cv::KeyPoint> keypointsCurrent;
    std::vector<cv::KeyPoint> keypointsPrev;
    cv::vector<cv::Point2f> pointsCurrent;
    cv::vector<cv::Point2f> pointsPrev;

    /*
    std::vector< DMatch > matches;
    std::vector< DMatch > matchesRev;
    */
    float keyRetainFactor;

    void featureBasedHomography();
    void flowBasedHomography();
    void init(cv::Mat &frame);
    void process();




public:
    frameAligner();
    void add(cv::Mat &frame);
    void alignPrevFrame(cv::Mat &alignedPrev);
// Setters
    void setDetector(cv::Ptr<cv::FeatureDetector> idetector);
    void setDetectorSimple(char *detectorName);
    void setDescriptor(cv::Ptr<cv::DescriptorExtractor> idescriptor);
    void setDescriptorSimple(char* descriptorName);
    void setMatcher(cv::Ptr<cv::DescriptorMatcher> imatcher);
    void setMatcherSimple(char* matcherName);

};

#endif // FRAMEALIGNER_H
