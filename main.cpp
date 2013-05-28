#include <cvs_uavtargetdetectionapp.h>

int main(int argc, char *argv[])
{
    CVS_UAVTargetDetectionApp targetDetection(argc, argv);
    
    targetDetection.exec();

    return 0;
}
