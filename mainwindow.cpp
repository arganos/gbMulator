#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>


#include <SFML/Graphics.hpp>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create a SFML view inside the main window
    SFMLView = new MyCanvas(this, QPoint(0, 0), QSize(SCREEN_WIDTH*PIXEL_SIZE, SCREEN_HEIGHT*PIXEL_SIZE));
    SFMLView->show();

    thread = new QThread;
    worker = new gb_worker();

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


void MainWindow::keyPressEvent(QKeyEvent *event) {

    switch (event->key()){
        case Qt::Key_T:
            //cout << "T" << endl;
            worker->keyDown('T');
            break;
    case Qt::Key_E:
        //cout << "E" << endl;
        worker->keyDown('E');
        break;
    case Qt::Key_A:
        //cout << "A" << endl;
        worker->keyDown('A');
        break;
    case Qt::Key_B:
        //cout << "B" << endl;
        worker->keyDown('B');
        break;
    case Qt::Key_Up:
        //cout << "U" << endl;
        worker->keyDown('U');
        break;
    case Qt::Key_Down:
        //cout << "D" << endl;
        worker->keyDown('D');
        break;
    case Qt::Key_Right:
        //cout << "R" << endl;
        worker->keyDown('R');
        break;
    case Qt::Key_Left:
        //cout << "L" << endl;
        worker->keyDown('L');
        break;
  }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {

    switch (event->key()){
        case Qt::Key_T:
            //cout << "T" << endl;
            worker->keyUp('T');
            break;
    case Qt::Key_E:
        //cout << "E" << endl;
        worker->keyUp('E');
        break;
    case Qt::Key_A:
        //cout << "A" << endl;
        worker->keyUp('A');
        break;
    case Qt::Key_B:
        //cout << "B" << endl;
        worker->keyUp('B');
        break;
    case Qt::Key_Up:
        //cout << "U" << endl;
        worker->keyUp('U');
        break;
    case Qt::Key_Down:
        //cout << "D" << endl;
        worker->keyUp('D');
        break;
    case Qt::Key_Right:
        //cout << "R" << endl;
        worker->keyUp('R');
        break;
    case Qt::Key_Left:
        //cout << "L" << endl;
        worker->keyUp('L');
        break;
  }
}

void MainWindow::paintFrame(sf::VertexArray* vert)
{
    SFMLView->m_vertices = *vert;
}



void MainWindow::on_actionROM_triggered()
{
    romFileName = QFileDialog::getOpenFileName(this, tr("Open ROM File"), "/home", tr("*.gb"));
    cout << romFileName.toStdString() << endl;
    worker->romFileName = romFileName;
    worker->gb_reset();
}

void MainWindow::on_actionBIOS_triggered()
{
    biosFileName = QFileDialog::getOpenFileName(this, tr("Open BIOS File"), "/home", tr("*.gb"));
    worker->biosFileName = biosFileName;
    worker->gb_reset();
}
