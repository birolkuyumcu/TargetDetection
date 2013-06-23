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

void MainWindow::setModulePtrs(Preprocess* preprocessor, FrameAlignment* frameAligner,
                                CandidateDetector* candidateDetector, CandidateFilter* CandidateFilter,
                                AlarmGenerator *alarmGenerator)
{
    pPreprocessor = preprocessor;
    pframeAligner = frameAligner;
    pCandidateFilter = CandidateFilter;
    pCandidateDetector = candidateDetector;
    pAlarmGenerator = alarmGenerator;

    preprocessor->getSettings(preprocessSettings);


    fillSettings();
}

void MainWindow::fillpreprocessorSettings()
{
    if(preprocessSettings.method == HistEq)
    {
        ui->comboBox_preprocessMethod->setCurrentIndex(0);
    }
    else if(preprocessSettings.method == GoF)
    {
        ui->comboBox_preprocessMethod->setCurrentIndex(1);
    }
}

void MainWindow::fillSettings()
{
    //preprocessor
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

void MainWindow::on_button_generalChange_clicked()
{

}

void MainWindow::on_button_generalSave_clicked()
{

}

void MainWindow::on_button_preprocessChange_clicked()
{
    if(ui->comboBox_preprocessMethod->currentText() == "HistEq")
    {
        preprocessSettings.method = HistEq;
    }
    else if(ui->comboBox_preprocessMethod->currentText() == "GoF")
    {
        preprocessSettings.method = GoF;
    }


    pPreprocessor->set(preprocessSettings);
}

void MainWindow::on_button_preprocessSave_clicked()
{
    on_button_preprocessChange_clicked();
    pPreprocessor->saveSettings();
}
