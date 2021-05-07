#include "IMUReadThread.h"
#include <QDebug>
#include <thread>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "lpsensor/LpmsSensorI.h"
//#include "LpmsSensorI.h"
#include "lpsensor/LpmsSensorManagerI.h"

bool init =false;
float Eulerangle[3];
float RadEulerangle[3];
float Quater4[4];
LpmsSensorManagerI* manager;
LpmsSensorI* lpms;
ImuData d;   //ImuData类型
TypfileIOBuf curDat;          //TypfileIOBuf是定义好的结构体  实例化为一个对象 curDat
QMutex mutex;
bool isImuConnected =false;
bool isImu1Connected =false;
bool thread1ok =false;
extern QQueue <TypfileIOBuf> fileIOBuf;  //引入别处定义的一个队列，此处再次声明
extern bool init1;
extern ImuData d1;
extern LpmsSensorManagerI* manager1;
extern LpmsSensorI* lpms1;
extern float Eulerangle1[3];
extern float Quater41[4];
extern float q1,q2,q3,q4;
extern float qq3;

IMUReadThread::IMUReadThread()
{
      //已经在构造这个线程了，stopped当然是false
      stopped = false;
}

//int tick50 = 50;

void IMUReadThread::run()  //继承后重写QThread的run函数
 {
     while(!stopped)
     {
         mutex.lock();     //给当前线程上互斥锁
         if(init == false)
         {
             init = true;
             manager = LpmsSensorManagerFactory();
             // 传感器管理员添加设备："00:04:3E:9B:A2:EB"
             lpms = manager->addSensor(DEVICE_LPMS_B2, "00:04:3E:9B:A2:EB");
         }

         if (lpms->getConnectionStatus() == SENSOR_CONNECTION_CONNECTED &&
             lpms->hasImuData())
         {
             thread1ok = true;
             isImuConnected = true;
             d = lpms->getCurrentData();
             lpms->getEulerAngle(Eulerangle);   //获取欧拉角存放到一个临时数组里
             lpms->getQuaternion(Quater4);      //获取四元素存放到一个临时数组里
             //取出临时数组里的数据放到结构体的成员变量中
             curDat.eular[0] = Eulerangle[0];
             curDat.eular[1] = Eulerangle[1];
             curDat.eular[2] = Eulerangle[2];

             //把第一个传感器的欧拉角怎么写入了四元素里？后面要把欧拉角转为四元素？
             q1 = Eulerangle[0];
             q2 = Eulerangle[1];
             q3 = Eulerangle[2];

             //取出临时数组里的数据放到结构体的成员变量中
             curDat.quater[0] = Quater4[0];
             curDat.quater[1] = Quater4[1];
             curDat.quater[2] = Quater4[2];
             curDat.quater[3] = Quater4[3];

             emit IMUDatSend(Eulerangle[0],Eulerangle[1],Eulerangle[2]);    //发出IMUDatSend这个信号
             emit IMUQSend(d.q[0],d.q[1],d.q[2],d.q[3]);                    //发出IMUDatSend这个信号
         }

         if(init1 == false)
         {
             init1 = true;
             manager1 = LpmsSensorManagerFactory();
             lpms1 = manager1->addSensor(DEVICE_LPMS_B2, "00:04:3E:9B:A3:1A");//"00:04:3E:9B:A3:1A"
             lpms1->startMagCalibration();
             lpms1->checkMagCal(1.0f);
             lpms1->stopMagCalibration();
         }

         if (lpms1->getConnectionStatus() == SENSOR_CONNECTION_CONNECTED &&
             lpms1->hasImuData())
         {
             isImu1Connected = true;
             d1 = lpms1->getCurrentData();
             lpms1->getEulerAngle(Eulerangle1);
             lpms1->getQuaternion(Quater41);

             curDat.eular1[0] = Eulerangle1[0];
             curDat.eular1[1] = Eulerangle1[1];
             curDat.eular1[2] = Eulerangle1[2];

             curDat.quater1[0] = Quater41[0];
             curDat.quater1[1] = Quater41[1];
             curDat.quater1[2] = Quater41[2];
             curDat.quater1[3] = Quater41[3];

             QDateTime current_date_time = QDateTime::currentDateTime();
             curDat.curTime = current_date_time.toString("hh:mm:ss.zzz");
             curDat.qa[0] = Eulerangle[0];  //qa代表传感器联合的四元素数组
             curDat.qa[1] = Eulerangle[1];
             curDat.qa[2] = Eulerangle[2];
             curDat.qa[3] = qq3;
         }
//         tick50--;
//         if(tick50==0)
//         {
         if(isImuConnected&&isImu1Connected ==true)
         {
            fileIOBuf.enqueue(curDat); //在队列尾部添加一个元素, 此处插入一个结构体。如果只连了一个IMU,不会添加元素
         }
//             tick50 = 50;
//         }
         mutex.unlock();

         //此线程在循环体内每10ms执行一次，即获取传感器一次数据
         std::this_thread::sleep_for(std::chrono::milliseconds(10));  // jilu yici
     }
     stopped = false;
 }

 void IMUReadThread::stop()
 {

     manager->removeSensor(lpms);
     delete manager;

     stopped = true;
 }


 //设置线程IMUReadThread1的信息，在mainwindow.cpp用到 "A"
 void IMUReadThread::setMessage(QString message)
 {
     messageStr = message;
 }

 void IMUReadThread::printMessage()
 {
     qDebug()<<messageStr;
     sleep(1);
 }

// 这个函数没有用到
 void sensor_disp()
 {

     printf("这是sensor_disp");
     LpmsSensorManagerI* manager = LpmsSensorManagerFactory();
     LpmsSensorI* lpms = manager->addSensor(DEVICE_LPMS_B2, "00:04:3E:9B:A3:1A");
     while(1)
     {
         if (lpms->getConnectionStatus() == SENSOR_CONNECTION_CONNECTED &&
             lpms->hasImuData())
         {
             d = lpms->getCurrentData();
             lpms->getEulerAngle(Eulerangle);
             printf("Timestamp=%f, qW=%f, qX=%f, qY=%f, qZ=%f, x=%f, y=%f, z=%f\n",
                 d.timeStamp, d.q[0], d.q[1], d.q[2], d.q[3],
                 d.r[0], d.r[1], d.r[2]);
             printf("Timestamp=%f, Roll=%f, Pitch=%f, Yaw=%f\n",d.timeStamp,
                    Eulerangle[0],Eulerangle[1],Eulerangle[2]);
         }
         std::this_thread::sleep_for(std::chrono::milliseconds(1));
     }
     manager->removeSensor(lpms);
     delete manager;
 }

