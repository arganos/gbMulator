#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QDialog>

namespace Ui {
class debugWindow;
}

class debugWindow : public QDialog
{
    Q_OBJECT

public:
    explicit debugWindow(QWidget *parent = 0);
    ~debugWindow();

private:
    Ui::debugWindow *ui;
};

#endif // DEBUGWINDOW_H
