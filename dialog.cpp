#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>


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

void Dialog::on_pushButton_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,tr("Open Video Stream "),".",tr("Video Files(*avi *mp4 *mpg)"));
    if(!QFile::exists(filename))
        return;

    ui->lineEdit_videoFileName->setText(filename);
}
