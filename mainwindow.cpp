#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshImgProcessingImg(void* imgPtr)
{
    QImage qOutputImage;
    cv::Mat* outputImgPtr = (cv::Mat*)imgPtr;


    if(cvMat2QImage(*outputImgPtr, qOutputImage))
    {
        ui->labelOutputImage->setPixmap(QPixmap::fromImage(qOutputImage));
    }
}

bool MainWindow::cvMat2QImage(cv::Mat &src, QImage& dst)
{
    QImage::Format format;
    bool conversionResult = 1;

    if(src.channels() == 3)
    {
       // is color OpenCvBGR to RGB format
       cv::cvtColor(src, src, CV_BGR2RGB);
       format = QImage::Format_RGB888;
    }
    else if(src.channels() == 1)
    {
       // is grayscale
       format = QImage::Format_Indexed8;
    }
    else
    {
        conversionResult = 0;
    }

    if(conversionResult)
    {
        dst = QImage((const unsigned char*)(src.data), src.cols, src.rows, src.step, format);
    }

    return conversionResult;
}
