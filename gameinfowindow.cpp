#include "gameinfowindow.h"
#include "ui_gameinfowindow.h"

GameInfoWindow::GameInfoWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameInfoWindow)
{
    ui->setupUi(this);
}

GameInfoWindow::~GameInfoWindow()
{
    delete ui;
}
