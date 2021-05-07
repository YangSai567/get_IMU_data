#ifndef IMUREADTHREAD1_H
 #define IMUREADTHREAD1_H

 #include <QThread>
#include "lpsensor/LpmsSensorI.h"
#include "lpsensor/LpmsSensorManagerI.h"


class IMUReadThread1 : public QThread
 {
      Q_OBJECT
 public:
     IMUReadThread1();
     void setMessage(QString message);
     void stop();

 protected:
     void run();
     void printMessage();

 private:
     QString messageStr;
     volatile bool stopped;

signals:
     void IMUDatSend1(float,float,float);
     void IMUQSend(float,float,float,float);
 };

 #endif // THREAD_H
