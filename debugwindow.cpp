#include "debugwindow.h"
#include "ui_debugwindow.h"

debugWindow::debugWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::debugWindow)
{
    ui->setupUi(this);
}

debugWindow::~debugWindow()
{
    delete ui;
}
