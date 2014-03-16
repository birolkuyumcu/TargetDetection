#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/opencv.hpp>



Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_FramePushed()
{
    cv::Mat frame=frameBuffer.front();
     QImage img;
     Mat2QImage(frame,img);
     ui->OrjFrame->setPixmap(QPixmap::fromImage(img).scaled(ui->OrjFrame->size(),Qt::KeepAspectRatio) );
  //   QThread::msleep(100);
    frameBuffer.pop();
}

void Dialog::on_pushButton_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,tr("Open Video Stream "),".",tr("Video Files(*avi *mp4 *mpg)"));
    if(!QFile::exists(filename))
        return;

    ui->lineEdit_videoFileName->setText(filename);


}

void Dialog::Mat2QImage(cv::Mat src, QImage &dst)
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

void Dialog::on_startButton_clicked()
{
    QString filename=ui->lineEdit_videoFileName->text();
    reader=new FrameProducer(this);
    reader->openVideoFile(filename,&frameBuffer);
    connect(reader,SIGNAL(framePushed()),this,SLOT(on_FramePushed()));
    ui->tabWidget->setCurrentIndex(0);
    reader->start();
}
