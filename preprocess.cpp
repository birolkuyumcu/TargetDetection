#include "preprocess.h"

Preprocess::Preprocess()
{
    //default settings
    settings.method = GoF;
}


void Preprocess::set(PreprocessSettings& _settings)
{
    settings = _settings;
}

void Preprocess::process(cv::Mat& inputImage)
{
    //preprocessin will be done over image here!
}
