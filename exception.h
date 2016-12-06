/*
 * Target Detection 
 * 
 * Copyright (C) Volkan Salma volkansalma@gmail.com
 * 				 Birol Kuyumcu  bluekid70@gmail.com
 * GPL v3 - https://github.com/birolkuyumcu/TargetDetection
 */
 
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
