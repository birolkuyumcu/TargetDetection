#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "frameproducer.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    FrameProducer *reader;
    std::queue<cv::Mat> frameBuffer;
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:
    void on_FramePushed();
    
private slots:
    void on_pushButton_clicked();

    void on_startButton_clicked();

private:
    Ui::Dialog *ui;
    void Mat2QImage(cv::Mat src, QImage& dst);
};

#endif // DIALOG_H
