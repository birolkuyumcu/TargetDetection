/*
 * Target Detection 
 * 
 * Copyright (C) Volkan Salma volkansalma@gmail.com
 * 				 Birol Kuyumcu  bluekid70@gmail.com
 * GPL v3 - https://github.com/birolkuyumcu/TargetDetection
 */
 
#include "cameraretrieve.h"


CameraRetrieve::CameraRetrieve()
{
}

void CameraRetrieve::run()
{
    while(1)
    {
        qDebug()<<"Camera Frame Query and push to processing unit";

        QThread::sleep(1);
    }
}
