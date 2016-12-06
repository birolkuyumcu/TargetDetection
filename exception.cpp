/*
 * Target Detection 
 * 
 * Copyright (C) Volkan Salma volkansalma@gmail.com
 * 				 Birol Kuyumcu  bluekid70@gmail.com
 * GPL v3 - https://github.com/birolkuyumcu/TargetDetection
 */
 
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
