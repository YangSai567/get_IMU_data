#include "mainwindow.h"
#include <QApplication>

#include <cstdio>
#include <thread>

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "lpsensor/LpmsSensorI.h"
#include "lpsensor/LpmsSensorManagerI.h"

int main(int argc, char *argv[])
{
    glutInit(&argc,argv);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();  //进入事件循环，让窗口接收用户传给窗口的参数，比如键盘输入，鼠标点击，
}

