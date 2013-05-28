#include "cvs_uavtargetdetectionapp.h"

CVS_UAVTargetDetectionApp::CVS_UAVTargetDetectionApp(int argc, char *argv[]):a(argc, argv)
{
    settings.load();
    w.show();
}

void CVS_UAVTargetDetectionApp::exec()
{
    a.exec();
}
