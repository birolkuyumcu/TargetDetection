#include "cvs_uavtargetdetectionapp.h"

CVS_UAVTargetDetectionApp::CVS_UAVTargetDetectionApp(int argc, char *argv[]):a(argc, argv)
{
    settings.load();
    w.show();

    if(settings.streamType == VideoStream)
    {
        imageSource = new VideoRetrieve(&imgProcess);
    }
    else
    {
        imageSource = new CameraRetrieve();
    }

    imageSource->setFps(settings.retrieveFps);
    imageSource->start();

    imgProcess.start();
}

void CVS_UAVTargetDetectionApp::exec()
{
    a.exec();
}
