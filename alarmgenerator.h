/*
 * Target Detection 
 * 
 * Copyright (C) Volkan Salma volkansalma@gmail.com
 * 				 Birol Kuyumcu  bluekid70@gmail.com
 * GPL v3 - https://github.com/birolkuyumcu/TargetDetection
 */
 

#ifndef ALARMGENERATOR_H
#define ALARMGENERATOR_H

#include <opencv2/opencv.hpp>
#include "exception.h"

class AlarmGeneratorSettings
{
public:
    int dummy;
};

class AlarmGenerator
{
public:
    AlarmGenerator();
    void set(AlarmGeneratorSettings& _settings);
    void getSettings(AlarmGeneratorSettings& _settings);
    void saveSettings();
    bool loadSettings();
    void process(cv::Mat& inputImage);

private:
    AlarmGeneratorSettings settings;

};

#endif // ALARMGENERATOR_H
