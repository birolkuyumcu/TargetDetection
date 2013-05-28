#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QString>
#include <QDebug>

class Exception
{
private:
    QString moduleName;
public:
    Exception();
    void setModuleName(QString _moduleName);
    void showException(QString msg);
};

#endif // EXCEPTION_H
