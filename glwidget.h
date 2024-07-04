#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QOpenGLWidget>
#include <QPainter>
#include <SFML/Graphics.hpp>
#include "gb_core/gb_lcd.h"

using namespace std;
class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

    gb_lcd_state state;
    float colors[4][4] = {{1.0, 1.0, 1.0, 1.0},
                          {0.7, 0.7, 0.7, 1.0},
                          {0.3, 0.3, 0.3, 1.0},
                          {0.0, 0.0, 0.0, 1.0}};
    void drawPixel(int x, int y, int col);
signals:
public slots:
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};
#endif // GLWIDGET_H
