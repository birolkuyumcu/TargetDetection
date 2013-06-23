#include "cvs_uavtargetdetectionapp.h"

CVS_UAVTargetDetectionApp::CVS_UAVTargetDetectionApp(int argc, char *argv[]):a(argc, argv)
{
    settings.load();

    w.setModulePtrs(&imgProcess.preprocess, &imgProcess.frameAligner,
                    &imgProcess.candidateDetector, &imgProcess.candidateFilter,
                    &imgProcess.alarmGenerator);
    w.show();

    imgProcess.connectGuiSlots(w);

    if(settings.streamType == VideoStream)
    {
        imageSource = new VideoRetrieve(&imgProcess);
        imageSource->setFps(settings.retrieveFps);
        ((VideoRetrieve*)imageSource)->openVideoFile(settings.videoFileName, 0, cv::Size(640, 480));
    }
    else
    {
        imageSource = new CameraRetrieve();
        imageSource->setFps(settings.retrieveFps);
    }


    imageSource->start();
    imgProcess.start();
}

void CVS_UAVTargetDetectionApp::exec()
{
    a.exec();
}
