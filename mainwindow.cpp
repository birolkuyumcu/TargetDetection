#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this); 

    exc.setModuleName("MainWindow");

    if(!loadSettings())
    {
        exc.showException("Settings could not be loaded");

        systemSettings.streamType = VideoStream;
        systemSettings.videoFileName = "D:/cvs/data/egt2.avi";
        systemSettings.retrieveFps = 20;
        systemSettings.viewFps = 30;
        systemSettings.imageWidth = 640;
        systemSettings.imageHeight = 480;
    }

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

void MainWindow::setModulePtrs(Preprocess* preprocessor,
                               AlignmentMatrixCalc *alignmentCalc,
                               FrameAlignment* frameAligner,
                               CandidateDetector* candidateDetector,
                               CandidateFilter* CandidateFilter,
                               AlarmGenerator *alarmGenerator)
{
    pPreprocessor = preprocessor;
    pframeAligner = frameAligner;
    pCandidateFilter = CandidateFilter;
    pCandidateDetector = candidateDetector;
    pAlarmGenerator = alarmGenerator;
    pAlignmentCalc = alignmentCalc;

    getSettings(systemSettings);
    pPreprocessor->getSettings(preprocessSettings);


    fillSettings();
}

/*
void MainWindow::setModulePtrs(ImageProcess* ipImgProcess)
{
    pImgProcess=ipImgProcess;
    setModulePtrs(&pImgProcess->preprocess,
                  &pImgProcess->alignmentCalc,
                  &pImgProcess->frameAligner,
                  &pImgProcess->candidateDetector,
                  &pImgProcess->candidateFilter,
                  &pImgProcess->alarmGenerator);
}
*/
void MainWindow::fillSystemSettings()
{
    if(systemSettings.streamType == VideoStream)
    {
        ui->comboBox_streamSrc->setCurrentIndex(0);
    }
    else if(systemSettings.streamType == CameraStream)
    {
        ui->comboBox_streamSrc->setCurrentIndex(1);
    }

    ui->lineEdit_videoFileName->setText(systemSettings.videoFileName);
    ui->lineEdit_retrieveFps->setText(QString::number(systemSettings.retrieveFps));
    ui->lineEdit_viewFps->setText(QString::number(systemSettings.viewFps));
    ui->lineEdit_imgWidth->setText(QString::number(systemSettings.imageWidth));
    ui->lineEdit_imgHeight->setText(QString::number(systemSettings.imageHeight));
}

void MainWindow::fillpreprocessorSettings()
{
    if(preprocessSettings.method == HistEq)
    {
        ui->comboBox_preprocessMethod->setCurrentIndex(0);
    }
    else if(preprocessSettings.method == DoG)
    {
        ui->comboBox_preprocessMethod->setCurrentIndex(1);
    }
}

void MainWindow::fillSettings()
{

    fillSystemSettings();
    fillpreprocessorSettings();
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

void MainWindow::set(SystemSettings& _settings)
{
     systemSettings = _settings;
}

void MainWindow::getSettings(SystemSettings& _settings)
{
    _settings = systemSettings;
}

void MainWindow::saveSettings()
{
    QFile file("systemSettings.bin");

    if ( file.open(QIODevice::WriteOnly | QIODevice::Truncate)  )
    {
        QDataStream out(&file);
        out.writeRawData((const char*)&systemSettings, sizeof(SystemSettings));
        file.close();
    }
}

bool MainWindow::loadSettings()
{
    bool readResult = false;

    QFile file("systemSettings.bin");

    if ( file.open(QIODevice::ReadOnly) )
    {
        readResult = true;
        QDataStream in(&file);
        in.readRawData((char*)&systemSettings, sizeof(SystemSettings));

        systemSettings.videoFileName = "test.avi"; // problem while reading QString
        file.close();
    }

    return readResult;
}


void MainWindow::on_button_generalChange_clicked()
{

    if(ui->comboBox_streamSrc->currentText() == "VideoStream")
    {
        systemSettings.streamType = VideoStream;
    }
    else if(ui->comboBox_streamSrc->currentText() == "CameraStream")
    {
        systemSettings.streamType = CameraStream;
    }

    systemSettings.videoFileName = QString(ui->lineEdit_videoFileName->text());
    systemSettings.retrieveFps = ui->lineEdit_retrieveFps->text().toUInt();
    systemSettings.viewFps = ui->lineEdit_viewFps->text().toUInt();
    systemSettings.imageWidth = ui->lineEdit_imgWidth->text().toUInt();
    systemSettings.imageHeight = ui->lineEdit_imgHeight->text().toUInt();

    set(systemSettings);
}

void MainWindow::on_button_generalSave_clicked()
{
    on_button_generalChange_clicked();
    saveSettings();
}

void MainWindow::on_button_preprocessChange_clicked()
{
    if(ui->comboBox_preprocessMethod->currentText() == "HistEq")
    {
        preprocessSettings.method = HistEq;
    }
    else if(ui->comboBox_preprocessMethod->currentText() == "DoG")
    {
        preprocessSettings.method = DoG;
    }


    pPreprocessor->set(preprocessSettings);
}

void MainWindow::on_button_preprocessSave_clicked()
{
    on_button_preprocessChange_clicked();
    //pPreprocessor->saveSettings();
}


void MainWindow::on_pushButton_clicked()
{
   // ImageProcess    imgProcess; ve ImageRetrieve*  imageSource; ulaşamıyoruz ? on pause off imkanı yok !
}

void MainWindow::on_applyAlignmentButton_clicked()
{
    pAlignmentCalc->setDetectorSimple(ui->comboBoxDetector->currentText().toStdString().c_str());
    pAlignmentCalc->setDescriptorSimple(ui->comboBoxDiscriptor->currentText().toStdString().c_str());
    pAlignmentCalc->setMatcherSimple(ui->comboBoxMatcher->currentText().toStdString().c_str());

    if(ui->comboBoxDiscriptorHMethod->currentText() == "Feature Based")
    {
        pAlignmentCalc->setHomographyMethod(featureBased);
    }
    else
    {
        pAlignmentCalc->setHomographyMethod(flowBased);
    }

    if( ui->comboBoxDiscriptorHMethodC->currentText()=="Ransac")
    {
        pAlignmentCalc->setHomographyCalcMethod(CV_RANSAC);
    }
    else if( ui->comboBoxDiscriptorHMethodC->currentText()=="Least - Median")
    {
        pAlignmentCalc->setHomographyCalcMethod(CV_LMEDS);
    }
    else
    {
        pAlignmentCalc->setHomographyCalcMethod(0); // Regular
    }





}
