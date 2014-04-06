#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "frameproducer.h"
#include "frameconsumer.h"
#include "framealigner.h"
#include "alignmentmatrixcalc.h"
#include "framealignment.h"
#include "Test/TEST_frameAllignment.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    FrameProducer *reader;
    FrameConsumer *processor;
    AlignmentMatrixCalc calc;
    FrameAlignment aligner;
    CandidateDetector cDet;
    CandidateFilter cFilt;
    std::queue<cv::Mat> frameBuffer;
    std::queue<cv::Mat> processedFrameBuffer;
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:
    void on_FramePushed();
    void on_FrameProcessed();
    void on_ProcessingEnd();
    
private slots:
    void on_pushButton_clicked();

    void on_startButton_clicked();

    void on_buttonSaveParameters_clicked();

    void on_buttonLoadParameters_clicked();

private:
    Ui::Dialog *ui;
    void Mat2QImage(cv::Mat src, QImage& dst);
    void setParameters();
    std::string pFileName;
   // std::vector<std::string> parameterTexts;
    QVector<QString> parameterTexts;
};

#endif // DIALOG_H
