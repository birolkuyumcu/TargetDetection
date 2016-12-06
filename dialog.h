/*
 * Target Detection 
 * 
 * Copyright (C) Volkan Salma volkansalma@gmail.com
 * 				 Birol Kuyumcu  bluekid70@gmail.com
 * GPL v3 - https://github.com/birolkuyumcu/TargetDetection
 */
 
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "frameproducer.h"
#include "frameconsumer.h"
#include "framealigner.h"
#include "alignmentmatrixcalc.h"
#include "framealignment.h"
#include "Test/TEST_frameAllignment.h"
#include "dialogfullscreen.h"

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
    bool fullScreen;
    DialogFullScreen *fDialog;
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:
    void on_FramePushed();
    void on_FrameProcessed();
    void on_ProcessingEnd();
    void on_ReadingEnd();
    void onDoubleClicked();
signals:
    void doubleClicked();
    
private slots:
    void on_pushButton_clicked();
    void on_startButton_clicked();
    void on_buttonSaveParameters_clicked();
    void on_buttonLoadParameters_clicked();

    void on_Dialog_destroyed();

    void on_pushButton_FullScreen_clicked();

private:
    Ui::Dialog *ui;
    void Mat2QImage(cv::Mat src, QImage& dst);
    void setParameters();
    std::string pFileName;
    QVector<QString> parameterTexts;
protected:
    bool eventFilter(QObject *target, QEvent *event);
};

#endif // DIALOG_H
