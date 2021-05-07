#include "IMUReadThread1.h"
#include <QDebug>
#include <thread>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "lpsensor/LpmsSensorI.h"
#include "lpsensor/LpmsSensorManagerI.h"

bool init1 =false;
ImuData d1;
float Eulerangle1[3];
float Quater41[4];
LpmsSensorManagerI* manager1;
LpmsSensorI* lpms1;
extern QQueue <TypfileIOBuf> fileIOBuf;
extern TypfileIOBuf curDat;

IMUReadThread1::IMUReadThread1()
{
      stopped = false;
 }

  void IMUReadThread1::run()
 {
     while(!stopped)
     {
//         if(init1 == false)
//         {
//             init1 = true;
//             manager1 = LpmsSensorManagerFactory();
//             lpms1 = manager1->addSensor(DEVICE_LPMS_B2, "00:04:3E:9B:A2:EB");
//         }

//         if (lpms1->getConnectionStatus() == SENSOR_CONNECTION_CONNECTED &&
//             lpms1->hasImuData())
//         {
//             d1 = lpms1->getCurrentData();
//             lpms1->getEulerAngle(Eulerangle1);
//             lpms1->getQuaternion(Quater41);

//             curDat.eular1 = Eulerangle1;
//             curDat.quater1= Quater41;
//             QDateTime current_date_time = QDateTime::currentDateTime();
//             curDat.curTime1 = current_date_time.toString("hh:mm:ss.zzz");

             emit IMUDatSend1(Eulerangle1[0],Eulerangle1[1],Eulerangle1[2]);
//         }
         std::this_thread::sleep_for(std::chrono::milliseconds(10));
         //std::this_thread::sleep_for(std::chrono::milliseconds(1));
     }
     stopped = false;
 }


 void IMUReadThread1::stop()
 {

     manager1->removeSensor(lpms1);
     delete manager1;

     stopped = true;
 }

 //设置线程IMUReadThread1的信息，在mainwindow.cpp用到 "B"
 void IMUReadThread1::setMessage(QString message)
 {
     messageStr = message;
 }

 void IMUReadThread1::printMessage()
 {
     qDebug()<<messageStr;
     sleep(1);
 }

