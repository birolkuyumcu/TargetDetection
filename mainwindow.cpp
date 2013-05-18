#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cvNamedWindow( "My Window", 1 );
    IplImage *img = cvCreateImage( cvSize( 640, 480 ), IPL_DEPTH_8U, 1 );
    CvFont font;
    double hScale = 1.0;
    double vScale = 1.0;
    int lineWidth = 1;
    cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC,
              hScale, vScale, 0, lineWidth );
    cvZero(img);
    cvPutText( img, CV_VERSION, cvPoint( 200, 400 ), &font,
             cvScalar( 255, 255, 0 ) );
    cvShowImage( "My Window", img );
    cvWaitKey(500);

}

MainWindow::~MainWindow()
{
    delete ui;
}
