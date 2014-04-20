#include "dialogfullscreen.h"
#include "ui_dialogfullscreen.h"
#include<QKeyEvent>

DialogFullScreen::DialogFullScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFullScreen)
{
    ui->setupUi(this);
    connect(this,SIGNAL(doubleClicked()),SLOT(onDoubleClicked()));
    this->showFullScreen();
    ui->label->installEventFilter(this);
    ui->label->setWindowFlags(Qt::Window);
    ui->label->setScaledContents(true);
    ui->label->showFullScreen();
    ui->label->installEventFilter(this);
}

DialogFullScreen::~DialogFullScreen()
{
    delete ui;
}

void DialogFullScreen::setFrame(QImage img)
{
    ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(),Qt::KeepAspectRatio) );
}

void DialogFullScreen::onDoubleClicked()
{
  ui->label->setParent(this);
  ui->label->setWindowFlags(Qt::Widget);
  reject();
}

bool DialogFullScreen::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick)
    {
         emit doubleClicked();
    }
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if( keyEvent->key() == 16777216 ) // ESC key code how i dont know
            emit doubleClicked();
    }
    return QDialog::eventFilter(target,event);
}
