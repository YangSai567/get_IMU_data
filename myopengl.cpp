#include "myopengl.h"
#include <QApplication>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <QMatrix>
#include <QMouseEvent>
#include <QWheelEvent>
#include "lpsensor/LpmsSensorI.h"
#include "lpsensor/LpmsSensorManagerI.h"
#include "mainwindow.h"
#include <math.h>
#include <ft2build.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include FT_FREETYPE_H

#define MAX_CHAR        128

using namespace Ui;

float vertex_point[8][4]={0};
extern QVector3D EulerA,EulerA1;
extern float Eulerangle[3];
extern float RadEulerangle[3];
extern float degree2rad;
extern float Quater4[4];
extern float q1,q2,q3,q4;
float PastRadEulerangle[3];

QQueue <TypfileIOBuf> fileIOBuf;

GLint index_list[][4] = {
    0, 2, 3, 1,
    0, 4, 6, 2,
    0, 1, 5, 4,
    4, 5, 7, 6,
    1, 3, 7, 5,
    2, 6, 7, 3,
};

void* bitmap_fonts[7] = {
        GLUT_BITMAP_9_BY_15,
        GLUT_BITMAP_8_BY_13,
        GLUT_BITMAP_TIMES_ROMAN_10,
        GLUT_BITMAP_TIMES_ROMAN_24,
        GLUT_BITMAP_HELVETICA_10,
        GLUT_BITMAP_HELVETICA_12,
        GLUT_BITMAP_HELVETICA_18
};


GLfloat colors[][3] = { { 0.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0 },
    { 1.0, 0.0, 1.0 }, { 1.0, 1.0, 0.0 }, { 0.0, 1.0, 1.0 }};

myopenGL::myopenGL(QWidget *parent) : QGLWidget(parent),mScale(1)
{
    showNormal();
    startTimer(5);
//    PastRadEulerA = QVector3D(0.0f,0.0f,0.0f);
//    PastRadEulerA1 = QVector3D(0.0f,0.0f,0.0f);
//    connect(this, SIGNAL(IMUDatSend(float,float,float)), this, SLOT(IMUDatRecv(float,float,float)));
//    connect(this, SIGNAL(IMUDatSend1(float,float,float)), this, SLOT(IMUDatRecv1(float,float,float)));
}

void myopenGL::resizeGL(int width, int height)
{
    if(height == 0)
        height =1;
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,width/height,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void myopenGL::initializeGL()
{
    up4ps.setRow(0,{-1.0f, -0.3f, -1.0f,1.0f});
    up4ps.setRow(1,{1.0f, -0.3f, -1.0f,1.0f});
    up4ps.setRow(2,{-1.0f, 0.3f, -1.0f,1.0f});
    up4ps.setRow(3,{1.0f, 0.3f, -1.0f,1.0f});
    down4ps.setRow(0,{-1.0f, -0.3f, 1.0f,1.0f});
    down4ps.setRow(1,{1.0f, -0.3f, 1.0f,1.0f});
    down4ps.setRow(2,{-1.0f, 0.3f, 1.0f,1.0f});
    down4ps.setRow(3,{1.0f, 0.3f, 1.0f,1.0f});
    glShadeModel(GL_SMOOTH);

    glClearColor(1.0f,1.0f,1.0f,0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3,GL_FLOAT,0,vertex_point);
    glColorPointer(3,GL_FLOAT,0,colors);

}
void myopenGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-7.0f);
    rotation.setScalar(Quater4[0]);
    rotation.setVector(Quater4[1],Quater4[3],-Quater4[2]);
    QMatrix4x4 projection;
    projection.rotate(rotation);
    projection.scale(mScale);
    int i = 0, j=0;
    float *p;
    p=(up4ps*projection).data();
    for(i=0;i<4;i++){
        for(j=0;j<4;j++,p++){
        vertex_point[j][i]=*p;
        }
    }
    p=(down4ps*projection).data();
    for(i=0;i<4;i++){
        for(j=0;j<4;j++,p++){
        vertex_point[j+4][i]=*p;
        }
    }
    glColor3f(0.0f, 1.0f, 0.0f);

        glBegin(GL_QUADS); // 绘制四边形
        for (i = 0; i < 6; ++i)         // 有六个面，循环六次
        {
            glColor3f(colors[i][0], colors[i][1], colors[i][2]);
            for (j = 0; j < 4; ++j)     // 每个面有四个顶点，循环四次
            {
                glVertex3fv(vertex_point[index_list[i][j]]);
            }
        }
    glEnd();

    glTranslatef(0.0f,0.0f,0.0f);
    glRasterPos3i(-2,-2,2);
    QString str;
    int QstrIndex =0;
    str.sprintf("Roll:%.1f,Pitch:%.1f,Yaw:%.1f",Eulerangle[0],Eulerangle[1],Eulerangle[2]);
    while (QstrIndex<str.length()) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str.at(QstrIndex).toLatin1());
        QstrIndex++;
    }
}

void myopenGL::timerEvent(QTimerEvent *)
{
    this->update();
    updateGL();
}


void myopenGL::print_bitmap_string(void* font,const char* s)
{
    if (s && strlen(s)) {
        while (*s) {
            glutBitmapCharacter(font, *s);
            s++;
        }
    }
}

void myopenGL::IMUDatRecv(float roll,float pitch,float yaw)
{
    rotationFromIMU.fromEulerAngles(roll,pitch,yaw);
}

