#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include "imageprocess.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void connectImageRefreshSlot(ImageProcess &imgProcess);
    ~MainWindow();

public slots:
    void refreshOutputImage(cv::Mat &outputImage);
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
