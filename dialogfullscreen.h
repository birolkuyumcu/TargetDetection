#ifndef DIALOGFULLSCREEN_H
#define DIALOGFULLSCREEN_H

#include <QDialog>
#include <QDebug>

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
public slots:
    void onDoubleClicked();
signals:
    void doubleClicked();

private:
    Ui::DialogFullScreen *ui;
protected:
    bool eventFilter(QObject *target, QEvent *event);
};

#endif // DIALOGFULLSCREEN_H
