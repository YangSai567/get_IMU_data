#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QPushButton>
  #include <QDialog>
  #include <QCloseEvent>
#include <QtOpenGL>
  #include "IMUReadThread.h"
#include "IMUReadThread1.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <QOpenGLWidget>
#include "QOpenGLFunctions"
#include "myopengl.h"
#include "myopengl1.h"
#include "UdpTxThread.h"

#include <QMainWindow>


struct TypfileIOBuf {
    float  eular[3];
    float quater[4];
    float  eular1[3];
    float quater1[4];
    float qa[4];  //传感器联合的四元素
    QString    curTime;
};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    IMUReadThread *threadA;
    IMUReadThread1 *threadB;
    UdpTxThread *nethread;
    myopenGL *s;
    myopenGL1 *s1;
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);
    void DispIMUDatText(float a,float b,float c);
    void timerEvent(QTimerEvent *event);
    void FileDatLog();
    void FileDatCreate();

private slots:
    void close();

    void on_threadAButton_clicked();

    void on_threadBButton_clicked();

    void on_quitButton_clicked();

    void on_netStart_clicked();

    void on_datRecBtn_clicked();

    // 校准IMU
    void startCalWaitBar(int );

    void startSetWaitBar(int );

    void calTimerUpdate();

    void on_offsetButton_clicked();

    void on_angleCalButton_clicked();

    void on_offerset1Button_clicked();

private:
    QPushButton *threadAButton;
    QPushButton *threadBButton;
    QPushButton *quitButton;
    Ui::MainWindow *ui;
    QProgressDialog *calProgress;
    QTimer *calTimer;
    int calTime;
    int calMaxTime;

    char calibrateFLAG;
};

#endif // MAINWINDOW_H
