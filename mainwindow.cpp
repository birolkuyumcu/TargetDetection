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

void MainWindow::refreshOutputImage(cv::Mat &outputImage)
{
    cv::imshow("Output!", outputImage);
}

void MainWindow::connectImageRefreshSlot(ImageProcess &imgProcess)
{
    QObject::connect(&imgProcess, SIGNAL(pushFrameToView(cv::Mat&)),
                     this, SLOT(refreshOutputImage(cv::Mat&)));
}
