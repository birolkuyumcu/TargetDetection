#ifndef SYSTEMSETTINGS_H
#define SYSTEMSETTINGS_H

#include <QString>
#include <QtSql/QSqlDatabase>

#include <exception.h>

enum StreamType
{
    VideoStream,
    CameraStream
};

class SystemSettings
{
private:

    QSqlDatabase db;
    Exception    exc;

public:
    SystemSettings();
    void loadDefaultParameters();
    void load();
    void save();


    QString videoFileName;
    StreamType streamType;
    int retrieveFps;
    int viewFps;
    int imageWidth;
    int imageHeight;

};

#endif // SYSTEMSETTINGS_H
