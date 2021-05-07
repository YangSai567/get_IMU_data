#ifndef IMUREADTHREAD_H
 #define IMUREADTHREAD_H

#include <QThread>
#include "lpsensor/LpmsSensorI.h"
#include "lpsensor/LpmsSensorManagerI.h"


class IMUReadThread : public QThread  //一个线程类，基类是QThread
 {
      Q_OBJECT
 public:
     IMUReadThread();
     void setMessage(QString message);
     void stop();

 protected:
     void run();
     void printMessage();


 private:
     QString messageStr;
     volatile bool stopped;

signals:
     void IMUDatSend(float,float,float);     //欧拉角
     void IMUQSend(float,float,float,float); //四元素
 };

 #endif // THREAD_H
