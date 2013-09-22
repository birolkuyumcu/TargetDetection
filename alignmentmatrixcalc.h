#ifndef ALIGNMENTMATRIXCALC_H
#define ALIGNMENTMATRIXCALC_H

#include "opencv2/nonfree/nonfree.hpp"
#include <opencv2/opencv.hpp>
#include "exception.h"

enum HomograpyMethod
{
    featureBased,
    flowBased
};

enum ProcessStage
{
    firstPass,
    secondPass,
    onGoing
};


enum MatchingType
{
    normalMatch,
    knnMatch,
    radiusMatch
};

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
    bool getHomography(cv::Mat& gHomography);
    void reset();

private:
    AlignmentMatrixCalcSettings settings;
    Exception                   exc;

    //
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
    MatchingType matchType;
    float maxRatio;
    float maxRadius;
    cv::Ptr<cv::FeatureDetector> detector;
    cv::Ptr<cv::DescriptorExtractor> descriptor;
    cv::Ptr<cv::DescriptorMatcher> matcher;
    std::vector<cv::KeyPoint> keypointsCurrent;
    std::vector<cv::KeyPoint> keypointsPrev;
    cv::vector<cv::Point2f> pointsCurrent;
    cv::vector<cv::Point2f> pointsPrev;
    float keyRetainFactor;
    bool isHomographyCalc;
    ProcessStage stage;
    int numOfPointsMin;

    //
    void featureBasedHomography();
    void flowBasedHomography();
    void init(cv::Mat &frame);
    bool run();
    void symmetryTest(std::vector< cv::DMatch >& matchesPrevToCurrent,std::vector< cv::DMatch >& matchesCurrentToPrev,std::vector< cv::DMatch >& matchesPassed);
    void symmetryTest(std::vector<std::vector<cv::DMatch> >&kmatchesPrevToCurrent,std::vector<std::vector<cv::DMatch> >& kmatchesCurrentToPrev,std::vector< cv::DMatch >& matchesPassed);
    void ratioTest(std::vector<std::vector<cv::DMatch> >&kmatches);
    bool isHomographyValid();
    void wayBack();
    //
public:
    void setDetector(cv::Ptr<cv::FeatureDetector> idetector);
    void setDetectorSimple(const char *detectorName);
    void setDescriptor(cv::Ptr<cv::DescriptorExtractor> idescriptor);
    void setDescriptorSimple(const char* descriptorName);
    void setMatcher(cv::Ptr<cv::DescriptorMatcher> imatcher);
    void setMatcherSimple(const char* matcherName);
    void setHomographyMethod(HomograpyMethod ihMethod);
    void setHomographyCalcMethod(int ihomographyCalcMethod);
    void setMatchingType(MatchingType iType);
    //


};

#endif // ALIGNMENTMATRIXCALC_H
