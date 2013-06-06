#ifndef IMAGERETRIEVE_H
#define IMAGERETRIEVE_H

#include <QThread>
#include <QDebug>
#include "imageprocess.h"
#include <exception.h>

class ImageRetrieve : public QThread
{
protected:
    Exception exc;
    ImageProcess* pImgProcess;
    int fps;
private:

public:
    ImageRetrieve();
    ~ImageRetrieve();

    virtual void run() = 0;

    void setFps(int _fps)
    {
        fps = _fps;
    }

};


#endif // IMAGERETRIEVE_H
