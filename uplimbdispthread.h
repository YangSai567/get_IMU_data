#ifndef UPLIMBDISPTHREAD_H
#define UPLIMBDISPTHREAD_H

#include <GL/glut.h>
#include <stdlib.h>
#include <QWidget>
#include <GL/gl.h>
#include <GL/glu.h>
#include<QOpenGLWidget>
#include"QOpenGLFunctions"
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QGLWidget>
#include <QVector2D>
#include <QQuaternion>
#include <QMatrix4x4>
#include <ft2build.h>

class uplimbDispThread: public QGLWidget
{
public:
     explicit  uplimbDispThread(QWidget *parent = nullptr);
 //   explicit uplimbDispThread();

protected:

    void init();
    void initializeGL();
    void paintGL();
    void timerEvent(QTimerEvent *event);
//    void reshape(int , int );
//    void DrawCoord(int, float , float, float);
//    void display(void);
//    void keyboard(unsigned char, int, int);

};

#endif // UPLIMBDISPTHREAD_H
