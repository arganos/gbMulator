#include "mycanvas.h"
#include <iostream>
#include <string>
#include <QDir>
MyCanvas::MyCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size) : QSFMLCanvas(Parent, Position, Size)
{
}
void MyCanvas::OnInit()
{

}
void MyCanvas::OnUpdate()
{
    // Clear screen
    RenderWindow::clear(sf::Color(0, 128, 0));

    // Draw it
    RenderWindow::draw(m_vertices);
}
