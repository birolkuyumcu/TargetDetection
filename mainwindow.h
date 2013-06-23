#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>

#include "preprocess.h"
#include "framealignment.h"
#include "candidatedetector.h"
#include "candidatefilter.h"
#include "alarmgenerator.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setModulePtrs(Preprocess* preprocessor, FrameAlignment* frameAligner,
                       CandidateDetector* candidateDetector, CandidateFilter* pCandidateFilter,
                       AlarmGenerator* alarmGenerator);
    
private:
    Ui::MainWindow      *ui;
    Preprocess*         pPreprocessor;
    FrameAlignment*     pframeAligner;
    CandidateDetector*  pCandidateDetector;
    CandidateFilter*    pCandidateFilter;
    AlarmGenerator*     pAlarmGenerator;

    PreprocessSettings preprocessSettings;

    bool cvMat2QImage(cv::Mat &src, QImage& dst);
    void fillSettings();
    void fillpreprocessorSettings();

public slots:
    void refreshImgProcessingImg(void* imgPtr);
private slots:
    void on_button_generalChange_clicked();
    void on_button_generalSave_clicked();
    void on_button_preprocessChange_clicked();
    void on_button_preprocessSave_clicked();
};

#endif // MAINWINDOW_H
