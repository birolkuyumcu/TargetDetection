#include "preprocess.h"
#include <QString>

Preprocess::Preprocess()
{
    exc.setModuleName("Preprocess");

    //default settings
    if(!loadSettings())
    {
        exc.showException("Settings could not loaded");

        settings.method = HistEq;
        settings.DoGSigma1 = 1.6;
        settings.DoGSigma2 = 1.9;
    }
}

void  Preprocess::getSettings(PreprocessSettings& _settings)
{
    _settings = settings;
}

void Preprocess::set(PreprocessSettings& _settings)
{
    settings = _settings;
}

void Preprocess::saveSettings()
{
    QFile file("preprocessSettings.bin");

    if ( file.open(QIODevice::WriteOnly | QIODevice::Truncate)  )
    {
        QDataStream out(&file);
        out.writeRawData((const char*)&settings, sizeof(PreprocessSettings));
        file.close();
    }
}

bool Preprocess::loadSettings()
{
    bool readResult = false;

    QFile file("preprocessSettings.bin");

    if ( file.open(QIODevice::ReadOnly) )
    {
        readResult = true;
        QDataStream in(&file);
        in.readRawData((char*)&settings, sizeof(PreprocessSettings));
        file.close();
    }

    return readResult;
}

void Preprocess::process(cv::Mat& inputImage)
{

    if(inputImage.channels() != 1)
    {
        cv::cvtColor(inputImage, inputImage, CV_BGR2GRAY);
    }

    if(settings.method == DoG)
    {
        inputImage.convertTo(inputImage32f, CV_32F);

        cv::GaussianBlur(inputImage32f, dog_1, cv::Size(), settings.DoGSigma1);
        cv::GaussianBlur(inputImage32f, dog_2, cv::Size(), settings.DoGSigma2);

        cv::subtract(dog_1, dog_2, inputImage32f);

        inputImage32f.convertTo(inputImage, CV_8U);

        //filterResponse.convertTo(filterResponse, CV_8U);

    }
    else if(settings.method == HistEq)
    {
        cv::equalizeHist(inputImage, inputImage);
    }

}
