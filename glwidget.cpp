#include "glwidget.h"
#include "gb_core/gb_lcd.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
}

void GLWidget::initializeGL() {
    glClearColor(0, 0, 0, 1);
}
void GLWidget::resizeGL(int w, int h){
}


void GLWidget::drawPixel(int x, int y, int col) {
    glBegin(GL_QUADS);
    glColor3f(colors[col][0], colors[col][1], colors[col][2]);
    glVertex2f( (float) x, (float) y);
    glVertex2f( (float) x, (float) y + 1.0f);
    glVertex2f( (float) x + 1.0f, (float) y + 1.0f);
    glVertex2f( (float) x + 1.0f, (float) y);
    glEnd();

}

void GLWidget::paintGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,0,0);
    for (int x = 0; x < SCREEN_WIDTH; x++)
        for (int y = 0; y < SCREEN_WIDTH; y++)
            drawPixel(x,y,state.screen[x][y]);

    glFlush();  // Render now
}

