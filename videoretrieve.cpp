#include "videoretrieve.h"
#include <QThread>

VideoRetrieve::VideoRetrieve()
{
}

void VideoRetrieve::run()
{
    while(1)
    {
        qDebug()<<"Video Frame Query and push to processing unit";

        QThread::sleep(1);
    }
}
