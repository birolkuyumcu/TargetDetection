#ifndef CVUTILITY_H
#define CVUTILITY_H
#include <QImage>
#include <QFile>
#include <QMessageBox>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


void Mat2QImage(cv::Mat src, QImage& dst);
cv::Mat Qimread(const QString& filename, int flags=1 );
cv::Mat QImage2Mat(const QImage & image);

#endif // CVUTILITY_H
