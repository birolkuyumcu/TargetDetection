#include "exception.h"

Exception::Exception()
{
    moduleName = "";
}

void Exception::setModuleName(QString _moduleName)
{
    moduleName = _moduleName;
}

void Exception::showException(QString msg)
{
    qDebug()<<moduleName<<":"<<msg;
}
