#include "cvs_uavtargetdetectionapp.h"

CVS_UAVTargetDetectionApp::CVS_UAVTargetDetectionApp(int argc, char *argv[]):a(argc, argv)
{

    exc.setModuleName("CVS_UAVTargetDetectionApp");

    //default settings

    w.setModulePtrs(&imgProcess.preprocess,
                    &imgProcess.alignmentCalc,
                    &imgProcess.frameAligner,
                    &imgProcess.candidateDetector,
                    &imgProcess.candidateFilter,
                    &imgProcess.alarmGenerator);
    w.show();

    imgProcess.connectGuiSlots(w);

    if(w.systemSettings.streamType == VideoStream)
    {
        imageSource = new VideoRetrieve(&imgProcess);
        imageSource->setFps(w.systemSettings.retrieveFps);
        ((VideoRetrieve*)imageSource)->openVideoFile(w.systemSettings.videoFileName, 0,
                                                     cv::Size(w.systemSettings.imageWidth,
                                                              w.systemSettings.imageHeight));
    }
    else
    {
        imageSource = new CameraRetrieve();
        imageSource->setFps(w.systemSettings.retrieveFps);
    }


    imageSource->start();
    imgProcess.start();
}

void CVS_UAVTargetDetectionApp::exec()
{
    a.exec();
}
