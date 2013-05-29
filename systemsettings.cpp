#include "systemsettings.h"

SystemSettings::SystemSettings()
{
    exc.setModuleName("SystemSettings");
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName("settings.db");
    loadDefaultParameters();
}


void SystemSettings::loadDefaultParameters()
{
    streamType = VideoStream;
    videoFileName = "test.avi";
    retrieveFps = 30;
    viewFps = 30;
    imageWidth = 640;
    imageHeight = 480;
}

void SystemSettings::load()
{
    if(!db.open())
    {
        exc.showException("Ayar veritabani acilamadi");
    }
}

void SystemSettings::save()
{

}
