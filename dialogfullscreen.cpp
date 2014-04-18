#include "dialogfullscreen.h"
#include "ui_dialogfullscreen.h"

DialogFullScreen::DialogFullScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFullScreen)
{
    ui->setupUi(this);
    this->setWindowState( Qt::WindowFullScreen);
    //ui->label->setWindowState( Qt::WindowFullScreen);
    ui->label->resize(this->size());
}

DialogFullScreen::~DialogFullScreen()
{
    delete ui;
}

void DialogFullScreen::setFrame(QImage img)
{
  ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(),Qt::KeepAspectRatio) );
}
