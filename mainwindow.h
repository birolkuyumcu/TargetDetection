#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>

#include "preprocess.h"
#include "framealignment.h"
#include "candidatedetector.h"
#include "candidatefilter.h"
#include "alarmgenerator.h"
#include "alignmentmatrixcalc.h"

namespace Ui
{
    class MainWindow;
}



enum StreamType
{
    VideoStream,
    CameraStream
};

class SystemSettings
{

public:
    QString videoFileName;
    StreamType streamType;
    int retrieveFps;
    int viewFps;
    int imageWidth;
    int imageHeight;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setModulePtrs(Preprocess* preprocessor,
                       AlignmentMatrixCalc* alignmentCalc,
                       FrameAlignment* frameAligner,
                       CandidateDetector* candidateDetector,
                       CandidateFilter* pCandidateFilter,
                       AlarmGenerator* alarmGenerator);


    SystemSettings                  systemSettings;
    
private:
    Ui::MainWindow                  *ui;
    Exception                       exc;

    Preprocess*                     pPreprocessor;
    FrameAlignment*                 pframeAligner;
    CandidateDetector*              pCandidateDetector;
    CandidateFilter*                pCandidateFilter;
    AlarmGenerator*                 pAlarmGenerator;
    AlignmentMatrixCalc*            pAlignmentCalc;

    PreprocessSettings              preprocessSettings;


    bool cvMat2QImage(cv::Mat &src, QImage& dst);
    void set(SystemSettings& _settings);
    void getSettings(SystemSettings& _settings);
    void saveSettings();

    bool loadSettings();
    void fillSettings();
    void fillpreprocessorSettings();
    void fillSystemSettings();

public slots:
    void refreshImgProcessingImg(void* imgPtr);
private slots:
    void on_button_generalChange_clicked();
    void on_button_generalSave_clicked();
    void on_button_preprocessChange_clicked();
    void on_button_preprocessSave_clicked();
    void on_pushButton_clicked();
    void on_applyAlignmentButton_clicked();
};

#endif // MAINWINDOW_H
