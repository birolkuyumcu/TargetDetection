#ifndef CVS_UAVTARGETDETECTIONAPP_H
#define CVS_UAVTARGETDETECTIONAPP_H

#include "mainwindow.h"
#include <QApplication>

#include "systemsettings.h"
#include "exception.h"
#include "videoretrieve.h"
#include "cameraretrieve.h"
#include "imageprocess.h"

class CVS_UAVTargetDetectionApp
{
private:

    QApplication a;
    MainWindow w;

    SystemSettings  settings;
    Exception       exc;

    ImageRetrieve*  imageSource;
    ImageProcess    imgProcess;

public:
    CVS_UAVTargetDetectionApp(int argc, char *argv[]);
    void exec();
};

#endif // CVS_UAVTARGETDETECTIONAPP_H
