#include "videoretrieve.h"
#include <QThread>

VideoRetrieve::VideoRetrieve(ImageProcess* _pImgProcess)
{
    pImgProcess = _pImgProcess;
}

void VideoRetrieve::run()
{
    while(1)
    {
        //"Video Frame Query and push to processing unit";


        cv::Mat testImage;
        testImage.create(100, 100, 0);

        pImgProcess->pushFrame(testImage);

        QThread::msleep(2);
    }
}
