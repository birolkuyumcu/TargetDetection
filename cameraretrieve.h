/*
 * Target Detection 
 * 
 * Copyright (C) Volkan Salma volkansalma@gmail.com
 * 				 Birol Kuyumcu  bluekid70@gmail.com
 * GPL v3 - https://github.com/birolkuyumcu/TargetDetection
 */
 
#ifndef CAMERARETRIEVE_H
#define CAMERARETRIEVE_H

#include "imageretrieve.h"

class CameraRetrieve : public ImageRetrieve
{
public:
    CameraRetrieve();
    void run();
};

#endif // CAMERARETRIEVE_H
