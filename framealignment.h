#ifndef FRAMEALIGNMENT_H
#define FRAMEALIGNMENT_H

#include <opencv2/opencv.hpp>
#include "exception.h"

class FrameAlignmentSettings
{
public:
    int dummy;
};

class FrameAlignment
{
public:
    FrameAlignment();
    void set(FrameAlignmentSettings& _settings);
    void getSettings(FrameAlignmentSettings& _settings);
    void saveSettings();
    bool loadSettings();
    void process(cv::Mat& inputImage);

private:
    FrameAlignmentSettings settings;
    Exception               exc;
};

#endif // FRAMEALIGNMENT_H
