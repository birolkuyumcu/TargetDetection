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

enum HomograpyMethod {featureBased,flowBased};

<<<<<<< HEAD
using namespace cv;

=======
/*
>>>>>>> 0c3d3e095355a8442dfc8c8b7d7198e7dad9713e
class frameAligner
{
protected:
//    Exception    exc;
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

    void featureBasedHomography();
    void flowBasedHomography();
    void init(cv::Mat &frame);
    void process();




public:
    frameAligner();
    void add(cv::Mat &frame);
    void alignPrevFrame(cv::Mat &alignedPrev);
// Setters
    void setDetector(cv::Ptr<FeatureDetector> idetector);
    void setDetectorSimple(char *detectorName);
    void setDescriptor(cv::Ptr<DescriptorExtractor> idescriptor);
    void setDescriptorSimple(char* descriptorName);
    void setMatcher(cv::Ptr<DescriptorMatcher> imatcher);
    void setMatcherSimple(char* matcherName);

};
*/
#endif // FRAMEALIGNER_H
