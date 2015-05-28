#include <QApplication>
#include <QThread>
#include <QObject>

#include "mycanvas.h"
#include "mainwindow.h"
#include "gb_core/gb_lcd.h"


#include "gb_worker.h"



int main(int argc, char *argv[])
{
    QApplication App(argc, argv);

    // Create the main window
    MainWindow* wnd = new MainWindow();

    wnd->setWindowTitle("gbmulator");
    wnd->resize(SCREEN_WIDTH*PIXEL_SIZE, SCREEN_HEIGHT*PIXEL_SIZE);
    wnd->show();






    return App.exec();

}
