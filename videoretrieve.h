#ifndef VIDEORETRIEVE_H
#define VIDEORETRIEVE_H

#include "imageretrieve.h"
#include <opencv2/opencv.hpp>

class VideoRetrieve : public ImageRetrieve
{
public:
    VideoRetrieve(ImageProcess* _pImgProcess);

    void run();
};

#endif // VIDEORETRIEVE_H
