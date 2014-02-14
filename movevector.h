#ifndef MOVEVECTOR_H
#define MOVEVECTOR_H

#include "opencv2/nonfree/nonfree.hpp"
#include <opencv2/opencv.hpp>
#include "exception.h"

class MoveVector
{
public:
    MoveVector();
    void process(cv::Mat& inputImage);
    void show();

private:
    Exception exc;
    cv::Mat currentFrame;
    cv::Mat prevFrame;
    std::vector<cv::Point2f> pointsPrev;
    int stepX;
    int stepY;
    double mDeltaX;
    double mDeltaY;
    int centerX;
    int centerY;


    void init();
    void calc();
};

#endif // MOVEVECTOR_H
