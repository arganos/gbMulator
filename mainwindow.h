#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QThread>

#include "gb_worker.h"
#include "gb_core/gb_lcd.h"
#include "mycanvas.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void paintFrame(sf::VertexArray* vert);

private:
    Ui::MainWindow *ui;
     MyCanvas* SFMLView;
};

#endif // MAINWINDOW_H
