#ifndef SYSTEMSETTINGS_H
#define SYSTEMSETTINGS_H

#include <QString>
#include <QtSql/QSqlDatabase>

#include <exception.h>

class SystemSettings
{
private:

    QSqlDatabase db;
    Exception    exc;

    QString videoFileName;
    int retrieveFps;
    int viewFps;
    int imageWidth;
    int imageHeight;
public:
    SystemSettings();
    void loadDefaultParameters();
    void load();
    void save();
};

#endif // SYSTEMSETTINGS_H
