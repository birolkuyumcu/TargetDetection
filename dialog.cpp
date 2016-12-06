/*
 * Target Detection 
 * 
 * Copyright (C) Volkan Salma volkansalma@gmail.com
 * 				 Birol Kuyumcu  bluekid70@gmail.com
 * GPL v3 - https://github.com/birolkuyumcu/TargetDetection
 */
 
#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QEvent>



Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    pFileName = "Parameters.xml";
    on_buttonLoadParameters_clicked();
    fullScreen = false;
    connect(this,SIGNAL(doubleClicked()),SLOT(onDoubleClicked()));
    ui->ProcFrame_3->installEventFilter(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_FramePushed()
{
    if(fullScreen == false )
    {
        cv::Mat frame_ = reader->frameBuffer.front();
        cv::Mat frame = frame_.clone();

        QImage img;

        Mat2QImage(frame, img);

        ui->OrjFrame->setPixmap(QPixmap::fromImage(img).scaled(ui->OrjFrame->size(),Qt::KeepAspectRatio) );
    }
}

void Dialog::on_FrameProcessed()
{

    QImage img;
    cv::Mat frame;
    if(fullScreen == false )
    {
        frame = processor->processedFrame2UiAbsDiff.clone();

        Mat2QImage(frame,img);
        ui->ProcFrame->setPixmap(QPixmap::fromImage(img).scaled(ui->ProcFrame->size(),Qt::KeepAspectRatio) );

        frame = processor->processedFrame2UiCandidates.clone();
        Mat2QImage(frame,img);
        ui->ProcFrame_2->setPixmap(QPixmap::fromImage(img).scaled(ui->ProcFrame_2->size(),Qt::KeepAspectRatio) );

        frame = processor->processedFrame2UiTargets.clone();
        Mat2QImage(frame,img);
        ui->ProcFrame_3->setPixmap(QPixmap::fromImage(img).scaled(ui->ProcFrame_3->size(),Qt::KeepAspectRatio) );
    }
    else
    {
        frame = processor->processedFrame2UiTargets.clone();
        Mat2QImage(frame,img);
        fDialog->setFrame(img);
    }

}

void Dialog::on_ProcessingEnd()
{
    // clean threads an make a start button actieve
    delete reader;
    delete processor;
    ui->startButton->setEnabled(true);
}

void Dialog::on_ReadingEnd()
{
    processor->isReadingEnd = true;
}

void Dialog::onDoubleClicked()
{
    on_pushButton_FullScreen_clicked();
}

void Dialog::on_pushButton_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,tr("Open Video Stream "),".",tr("Video Files(*avi *mp4 *mpg)"));

    if(!QFile::exists(filename))
    {
        return;
    }

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
    else
    {
        // Üsteki iki tip dışındaki formatlar gösterilmek için uygun değil...
        QMessageBox msgBox;
        msgBox.setText("Görüntü Kanal sayısı uyumsuz !");
        msgBox.exec();
        return;
    }
    dst=QImage((const unsigned char*)(src.data),src.cols,src.rows,src.step,f);
}


void Dialog::setParameters()
{
    parameterTexts.clear();
    parameterTexts.push_back(ui->comboBoxDetector->currentText());
    parameterTexts.push_back(ui->comboBoxDescriptor->currentText());
    parameterTexts.push_back(ui->comboBoxMatcher->currentText());
    parameterTexts.push_back(ui->comboBoxDescriptorHMethod->currentText());
    parameterTexts.push_back(ui->comboBoxDescriptorHMethodC->currentText());
}

bool Dialog::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick)
    {
         emit doubleClicked();
    }
    return QDialog::eventFilter(target,event);
}

void Dialog::on_startButton_clicked()
{

    QString filename=ui->lineEdit_videoFileName->text();
    reader=new FrameProducer(this);
    reader->openVideoFile(filename);
    connect(reader,SIGNAL(framePushed()),this,SLOT(on_FramePushed()));
    ui->tabWidget->setCurrentIndex(0);
    reader->start();

    processor =new FrameConsumer(this);

    processor->setBuffers(&reader->frameBuffer);

    connect(processor,SIGNAL(frameProcessed()),this,SLOT(on_FrameProcessed()));
    setParameters();
    processor->setParameters(parameterTexts);
    processor->start();

    // to avoid re-enter
    ui->startButton->setDisabled(true);

}

void Dialog::on_buttonSaveParameters_clicked()
{
    cv::FileStorage fs(pFileName, cv::FileStorage::WRITE);
    fs<<"Detector"<<ui->comboBoxDetector->currentText().toStdString();
    fs<<"Descriptor"<<ui->comboBoxDescriptor->currentText().toStdString();
    fs<<"Matcher"<<ui->comboBoxMatcher->currentText().toStdString();
    fs<<"HomographyMethod"<<ui->comboBoxDescriptorHMethod->currentText().toStdString();
    fs<<"HomographyCalcMethod"<<ui->comboBoxDescriptorHMethodC->currentText().toStdString();
    fs<<"FileName"<<ui->lineEdit_videoFileName->text().toStdString();

    fs.release();

}

void Dialog::on_buttonLoadParameters_clicked()
{
    cv::FileStorage fs(pFileName, cv::FileStorage::READ);
    std::string det, desc, match,hMet,hMetC,fileName ;
    fs["Detector"]>>det;
    fs["Descriptor"]>>desc;
    fs["Matcher"]>>match;
    fs["HomographyMethod"]>>hMet;
    fs["HomographyCalcMethod"]>>hMetC;
    fs["FileName"]>>fileName;

    fs.release();
    ui->comboBoxDetector->setCurrentText(det.c_str());
    ui->comboBoxDescriptor->setCurrentText(desc.c_str());
    ui->comboBoxMatcher->setCurrentText(match.c_str());
    ui->comboBoxDescriptorHMethod->setCurrentText(hMet.c_str());
    ui->comboBoxDescriptorHMethodC->setCurrentText(hMetC.c_str());
    ui->lineEdit_videoFileName->setText(fileName.c_str());

}

void Dialog::on_Dialog_destroyed()
{
    reader->terminate();
    processor->terminate();

}

void Dialog::on_pushButton_FullScreen_clicked()
{
    fullScreen = true ;
    fDialog = new DialogFullScreen();
    fDialog->exec();
    fullScreen = false;

}
