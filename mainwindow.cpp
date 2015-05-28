#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <SFML/Graphics.hpp>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create a SFML view inside the main window
    SFMLView = new MyCanvas(this, QPoint(0, 0), QSize(SCREEN_WIDTH*PIXEL_SIZE, SCREEN_HEIGHT*PIXEL_SIZE));
    SFMLView->show();

    QThread* thread = new QThread;
    gb_worker* worker = new gb_worker();

    worker->moveToThread(thread);
    connect(thread, SIGNAL(started()), worker, SLOT(run()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(worker, SIGNAL(frameCalculated(sf::VertexArray*)), this, SLOT(paintFrame(sf::VertexArray*)));
    thread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintFrame(sf::VertexArray* vert)
{

    SFMLView->m_vertices = *vert;
    //SFMLView->OnUpdate();
}
