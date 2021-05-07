#include "myopengl1.h"
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

float vertex_point1[8][4]={0};
extern QVector3D EulerA1;
extern float Eulerangle1[3];
extern float degree2rad;
extern float q1,q2,q3,q4;
extern float Quater41[4];

GLint index_list1[][4] = {
    0, 2, 3, 1,
    0, 4, 6, 2,
    0, 1, 5, 4,
    4, 5, 7, 6,
    1, 3, 7, 5,
    2, 6, 7, 3,
};


GLfloat colors1[][3] = { { 0.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0 },
    { 1.0, 0.0, 1.0 }, { 1.0, 1.0, 0.0 }, { 0.0, 1.0, 1.0 }};

myopenGL1::myopenGL1(QWidget *parent) : QGLWidget(parent),mScale(1)
{
    showNormal();
    startTimer(5);
//    PastRadEulerA = QVector3D(0.0f,0.0f,0.0f);
//    PastRadEulerA1 = QVector3D(0.0f,0.0f,0.0f);
//    connect(this, SIGNAL(IMUDatSend(float,float,float)), this, SLOT(IMUDatRecv(float,float,float)));
//    connect(this, SIGNAL(IMUDatSend1(float,float,float)), this, SLOT(IMUDatRecv1(float,float,float)));
}

void myopenGL1::resizeGL(int width, int height)
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


void myopenGL1::initializeGL()
{
    //行存储
//    up4ps.setRow(0,{-1.0f, -1.0f, -1.0f,1.0f});
//    up4ps.setRow(1,{1.0f, -1.0f, -1.0f,1.0f});
//    up4ps.setRow(2,{-1.0f, 1.0f, -1.0f,1.0f});
//    up4ps.setRow(3,{1.0f, 1.0f, -1.0f,1.0f});
//    down4ps.setRow(0,{-1.0f, -1.0f, 1.0f,1.0f});
//    down4ps.setRow(1,{1.0f, -1.0f, 1.0f,1.0f});
//    down4ps.setRow(2,{-1.0f, 1.0f, 1.0f,1.0f});
//    down4ps.setRow(3,{1.0f, 1.0f, 1.0f,1.0f});

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
    glVertexPointer(3,GL_FLOAT,0,vertex_point1);
    glColorPointer(3,GL_FLOAT,0,colors1);

}
void myopenGL1::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-7.0f);
    rotation.setScalar(Quater41[0]);
    rotation.setVector(-Quater41[1],Quater41[3],Quater41[2]);
    QMatrix4x4 projection;
    projection.rotate(rotation);
    projection.scale(mScale);
    int i = 0, j=0;
    float *p;
    p=(up4ps*projection).data();
    for(i=0;i<4;i++){
        for(j=0;j<4;j++,p++){
        vertex_point1[j][i]=*p;
        }
    }
    p=(down4ps*projection).data();
    for(i=0;i<4;i++){
        for(j=0;j<4;j++,p++){
        vertex_point1[j+4][i]=*p;
        }
    }
    glColor3f(0.0f, 1.0f, 0.0f);

        glBegin(GL_QUADS); // 绘制四边形
        for (i = 0; i < 6; ++i)         // 有六个面，循环六次
        {
            glColor3f(colors1[i][0], colors1[i][1], colors1[i][2]);
            for (j = 0; j < 4; ++j)     // 每个面有四个顶点，循环四次
            {
                glVertex3fv(vertex_point1[index_list1[i][j]]);
            }
        }
    glEnd();

    glTranslatef(0.0f,0.0f,0.0f);
    glRasterPos3i(-2,-2,2);
    QString str;
    int QstrIndex =0;
    str.sprintf("Roll:%.1f,Pitch:%.1f,Yaw:%.1f",Eulerangle1[0],Eulerangle1[1],Eulerangle1[2]);
    while (QstrIndex<str.length()) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str.at(QstrIndex).toLatin1());
        QstrIndex++;
    }
}

void myopenGL1::timerEvent(QTimerEvent *)
{
    this->update();
    updateGL();
}


void myopenGL1::print_bitmap_string(void* font,const char* s)
{
    if (s && strlen(s)) {
        while (*s) {
            glutBitmapCharacter(font, *s);
            s++;
        }
    }
}

void myopenGL1::IMUDatRecv1(float roll,float pitch,float yaw)
{
    rotationFromIMU1.fromEulerAngles(roll,pitch,yaw);
}
