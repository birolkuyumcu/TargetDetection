/*
 * Target Detection 
 * 
 * Copyright (C) Volkan Salma volkansalma@gmail.com
 * 				 Birol Kuyumcu  bluekid70@gmail.com
 * GPL v3 - https://github.com/birolkuyumcu/TargetDetection
 */
 
#ifndef CVS_UAVTARGETDETECTIONAPP_H
#define CVS_UAVTARGETDETECTIONAPP_H

#include "mainwindow.h"
#include <QApplication>

#include "exception.h"
#include "videoretrieve.h"
#include "cameraretrieve.h"
#include "imageprocess.h"


class CVS_UAVTargetDetectionApp
{
private:

    QApplication a;
    ImageProcess    imgProcess;
    Exception       exc;
    ImageRetrieve*  imageSource;
    MainWindow w;

public:
    CVS_UAVTargetDetectionApp(int argc, char *argv[]);
    void exec();
};

#endif // CVS_UAVTARGETDETECTIONAPP_H
