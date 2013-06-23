#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <opencv2/opencv.hpp>
#include <QFile>
#include <QDataStream>
#include "exception.h"

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
    void getSettings(PreprocessSettings& _settings);
    void saveSettings();
    bool loadSettings();
    void process(cv::Mat& inputImage);


private:
    PreprocessSettings  settings;
    Exception           exc;
};


#endif // PREPROCESS_H
