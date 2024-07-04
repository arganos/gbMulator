#ifndef MYCANVAS_H
#define MYCANVAS_H
#include "qsfmlcanvas.h"
#include <SFML/Graphics.hpp>
class MyCanvas : public QSFMLCanvas
{
    public :
        MyCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size);
    void OnInit();
    void OnUpdate();


    sf::VertexArray m_vertices;
};

#endif // MYCANVAS_H
