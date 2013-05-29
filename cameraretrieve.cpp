#include "cameraretrieve.h"


CameraRetrieve::CameraRetrieve()
{
}

void CameraRetrieve::run()
{
    while(1)
    {
        qDebug()<<"Camera Frame Query and push to processing unit";

        QThread::sleep(1);
    }
}
