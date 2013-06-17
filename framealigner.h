#ifndef FRAMEALIGNER_H
#define FRAMEALIGNER_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/video/tracking.hpp"
#include "exception.h"

enum HomograpyMethod {featureBased,flowBased};

/*
class frameAligner
{

private:
    cv::Mat currentFrame;
    cv::Mat prevFrame;
    cv::Mat descriptorsCurrent;
    cv::Mat descriptorsPrev;
    cv::Mat homography;
    HomograpyMethod hMethod;
    cv::Ptr<FeatureDetector> detector;
    cv::Ptr<DescriptorExtractor> descriptor;
    cv::Ptr<DescriptorMatcher> matcher;
    std::vector<KeyPoint> keypointsCurrent;
    std::vector<KeyPoint> keypointsPrev;
    vector<Point2f> pointsCurrent;
    vector<Point2f> pointsPrev;

    / *
    std::vector< DMatch > matches;
    std::vector< DMatch > matchesRev;
    * /
    float keyRetainFactor;

    cv::Mat featureBasedHomography();
    cv::Mat flowBasedHomography();
    void init(cv::Mat &frame);
    void process();




public:
    frameAligner();
    void add(cv::Mat &frame);



};
*/
#endif // FRAMEALIGNER_H
