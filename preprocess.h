#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <opencv2/opencv.hpp>
#include <QFile>
#include <QDataStream>
#include "exception.h"

enum PreprocessMethods
{
    HistEq,
    DoG
};

class PreprocessSettings
{
public:
    PreprocessMethods method;


    float               DoGSigma1;
    float               DoGSigma2;
};

class Preprocess
{
public:
    Preprocess();
    void set(PreprocessSettings& _settings);
    void getSettings(PreprocessSettings& _settings);
    void saveSettings();
    bool loadSettings();
    void process(cv::Mat& inputImage);


private:
    PreprocessSettings  settings;
    Exception           exc;

    cv::Mat             dog_1;
    cv::Mat             dog_2;
    cv::Mat             inputImage32f;
};


#endif // PREPROCESS_H
