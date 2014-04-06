#ifndef ALIGNMENTMATRIXCALC_H
#define ALIGNMENTMATRIXCALC_H

#include "opencv2/nonfree/nonfree.hpp"
#include <opencv2/opencv.hpp>
#include "exception.h"

/* HomograpyMethod means that calculation of homography based on
 * for featureBased ; find feature of each frame and calculete homography by using matching of them
 * for flowBased ; find feature of first Frame then find flow at second frame and calculete homography by using these pairs
 **/
enum HomograpyMethod
{
    featureBased,
    flowBased
};

/*
 * firstPass ; Initial stage of calculation gets only first frame
 * secondPass ; gets second frame calculate homography
 * onGoing ; second Frame -> first Frame then gets new second frame  calculate homography
 **/
enum ProcessStage
{
    firstPass,
    secondPass,
    onGoing
};

/* MatchingType for featurebased Homography calculation
 * for detail refer to OpenCv Documentation
 * radiusMatch not working - OpenCv issue -
 **/
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
    unsigned int numOfPointsMin;
    unsigned int errorCount;

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
    void setDetectorSimple(QString detectorName);
    void setDescriptor(cv::Ptr<cv::DescriptorExtractor> idescriptor);
    void setDescriptorSimple(QString descriptorName);
    void setMatcher(cv::Ptr<cv::DescriptorMatcher> imatcher);
    void setMatcherSimple(QString matcherName);
    void setHomographyMethod(HomograpyMethod ihMethod);
    void setHomographyCalcMethod(int ihomographyCalcMethod);
    void setMatchingType(MatchingType iType);
    //


};

#endif // ALIGNMENTMATRIXCALC_H
