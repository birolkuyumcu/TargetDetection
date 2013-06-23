#include "preprocess.h"

Preprocess::Preprocess()
{
    exc.setModuleName("Preprocess");

    //default settings
    if(!loadSettings())
    {
        exc.showException("Settings could not loaded");

        settings.method = GoF;
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
    //preprocessin will be done over image here!
}
