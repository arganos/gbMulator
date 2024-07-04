#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QThread>
#include <QWidget>
#include <QtGui>

#include "gb_worker.h"
#include "gb_core/gb_lcd.h"
#include "qsfmlcanvas.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

public slots:
    void paintFrame(gb_lcd_state state);

private slots:

    void on_actionROM_triggered();

    void on_actionBIOS_triggered();

private:
     Ui::MainWindow *ui;
     QSFMLCanvas* SFMLView;

     QThread* thread;
     gb_worker* worker;

     //Game relavant Parameters

     QString romFileName;
     QString biosFileName;
};

#endif // MAINWINDOW_H
