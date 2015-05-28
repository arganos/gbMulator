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
    // Rotate the sprite
    mySprite.rotate(myClock.getElapsedTime().asSeconds() * 100.f);
    // Draw it
    RenderWindow::draw(mySprite);
    myClock.restart();
}
