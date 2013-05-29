#ifndef IMAGERETRIEVE_H
#define IMAGERETRIEVE_H

#include <QThread>
#include <QDebug>

class ImageRetrieve : public QThread
{
private:
    int fps;

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
