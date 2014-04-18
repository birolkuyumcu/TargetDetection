#ifndef DIALOGFULLSCREEN_H
#define DIALOGFULLSCREEN_H

#include <QDialog>

namespace Ui {
class DialogFullScreen;
}

class DialogFullScreen : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFullScreen(QWidget *parent = 0);
    ~DialogFullScreen();
    void setFrame(QImage img);

private:
    Ui::DialogFullScreen *ui;
};

#endif // DIALOGFULLSCREEN_H
