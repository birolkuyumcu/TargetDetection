#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <opencv2/opencv.hpp>

enum PreprocessMethods
{
    HistEq,
    GoF
};

class PreprocessSettings
{
public:
    PreprocessMethods method;
};

class Preprocess
{
public:
    Preprocess();
    void set(PreprocessSettings& _settings);
    void process(cv::Mat& inputImage);

private:
    PreprocessSettings settings;
};


#endif // PREPROCESS_H
