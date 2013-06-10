#include "cvutility.h"

cv::Mat Qimread(const QString& filename, int flags)
{
    // return cv::imread(qPrintable(filename),flags);
     return cv::imread(QFile::encodeName(filename).data(),flags);
}

// Copyright (C) 2011, Mathieu Labbe - IntRoLab - Universite de Sherbrooke
// FindObject kodlarından
cv::Mat QImage2Mat(const QImage & image)
{
    cv::Mat dst;
    if(!image.isNull() && image.depth() == 32 && image.format() == QImage::Format_RGB32)
    {
        // assume RGB (3 channels)
        int channels = 3;
        dst = cv::Mat(image.height(), image.width(), CV_8UC3);
        unsigned char * data = dst.data;
    //    const IplImage test = dst;
        //printf("%d vs %d\n", dst.cols*int(dst.elemSize()), dst.step, 0);
        for(int y = 0; y < image.height(); ++y, data+=dst.cols*dst.elemSize())
        {
            for(int x = 0; x < image.width(); ++x)
            {
                QRgb rgb = image.pixel(x, y);
                data[x * channels+2] = qRed(rgb); //r
                data[x * channels+1] = qGreen(rgb); //g
                data[x * channels] = qBlue(rgb); //b
            }
        }
    }
    else
    {
        printf("Failed to convert image : depth=%d(!=32) format=%d(!=%d)\n", image.depth(), image.format(), QImage::Format_RGB32);
    }
    return dst;
}
