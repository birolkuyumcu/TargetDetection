#include "cvutility.h"

void Mat2QImage(cv::Mat src, QImage& dst)
{
    QImage::Format f;

    if(src.channels()==3){
       // Renkli ise OpenCvnin BGR okuduğunu RGB formatına çeviriyoruz
       cv::cvtColor(src,src,CV_BGR2RGB);
       f=QImage::Format_RGB888;
    }
    else if(src.channels()==1){
       // Siyah Beyaz ise Sadece formatı değiştiriyoruz.
       f=QImage::Format_Indexed8;
    }
    else{
        // Üsteki iki tip dışındaki formatlar gösterilmek için uygun değil...
        QMessageBox msgBox;
        msgBox.setText("Görüntü Kanal sayısı uyumsuz !");
        msgBox.exec();
        return;
    }
    dst=QImage((const unsigned char*)(src.data),src.cols,src.rows,src.step,f);
}

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
        printf("%d vs %d\n", dst.cols*int(dst.elemSize()), dst.step);
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
