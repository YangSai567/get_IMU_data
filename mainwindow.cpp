#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QHostAddress>
#include <QUdpSocket>

#include <cstdio>
#include <thread>
#include "IMUReadThread.h"
#include "IMUReadThread1.h"

#include <math.h>
#include "mainwindow.h"
#include "myopengl.h"


#include "lpsensor/LpmsSensorI.h"
#include "lpsensor/LpmsSensorManagerI.h"


extern float Quater4[4];
extern float Quater41[4];
extern float Eulerangle[3];
extern float Eulerangle1[3];
extern QQueue <TypfileIOBuf> fileIOBuf;
extern TypfileIOBuf curDat;
extern LpmsSensorI* lpms, *lpms1;
extern bool thread1ok;

float q1,q2,q3,q4;
float degree2rad;

bool angleCalFLAG = false;

QString ipaddr;
QString netport;
QString fileName;
float imu1PWR,imu2PWR;

bool recFlag;

MainWindow::MainWindow(QWidget *parent) :
    //初始化列表 注意这里是逗号
    QMainWindow(parent),  //指定父类，方便回收
    ui(new Ui::MainWindow)

 //开始实现构造函数
{
    threadA = new IMUReadThread;  //创建IMU读取线程
    threadB = new IMUReadThread1;
    nethread= new UdpTxThread;    //创建UDP通信线程
    recFlag = false;
    s = new myopenGL(this);
    setCentralWidget(s);
    s->setMinimumSize(400,400);
    threadA->setMessage("A");
    threadB->setMessage("B");
    degree2rad = (M_PI*180);      // pi
    startTimer(3000);             //一个3秒的计时器  每隔3秒干一件什么事？
    calibrateFLAG =0x0;
    q1=0;
    q2=0;
    q3=0;
    q4=0;
    ui->setupUi(this);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    threadA->stop();
    threadB->stop();
    nethread->stop();
    threadA->wait();
    threadB->wait();
    nethread->wait();
    event->accept();
}

void MainWindow::close()
{
    threadA->stop();
    threadB->stop();
    exit(0);
}

void MainWindow::timerEvent(QTimerEvent *)
{
    if(thread1ok == true)
    {
//        if ((lpms->getConnectionStatus() == SENSOR_CONNECTION_CONNECTED && lpms->hasImuData())&&
//             (lpms1->getConnectionStatus() == SENSOR_CONNECTION_CONNECTED && lpms1->hasImuData()))
//        {
            imu1PWR =lpms->getBatteryLevel();
            imu2PWR =lpms1->getBatteryLevel();
            QString imu1PWRstr = QString::number(imu1PWR)+"%";
            QString imu2PWRstr = QString::number(imu2PWR)+"%";
            ui->imu1pwrlabel->setText("IMU1:"+imu1PWRstr);
            ui->imu2pwrlabel->setText("IMU2:"+imu2PWRstr);
//        }
    }
        if(recFlag == true)
        {
            FileDatLog();
        }
}

// 校准IMU
void MainWindow::startCalWaitBar(int t)         //用时间t参数来创建校准进度条
{
    //显示一个对话进度条，设置各种参数
    calProgress = new QProgressDialog("Calibrating LPMS. Please wait..", QString() /* "Cancel" */, 0, t, this);
    calProgress->setWindowFlags(calProgress->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    calProgress->setWindowModality(Qt::WindowModal);
    calProgress->setMinimumWidth(400);
    calProgress->setAutoReset(false);
    calProgress->show();

    calTimer = new QTimer(this);
    connect(calTimer, SIGNAL(timeout()), this, SLOT(calTimerUpdate()));
    calTimer->start(500);               // 500毫秒后启动定时器
    calMaxTime = t;                     //校准最大时间
    calTime = 0;                        //校准时间清零
}

void MainWindow::startSetWaitBar(int t)      //进一步设置进度条（更新校准的数据）
{
    calProgress = new QProgressDialog("Updating Cal Data. Please wait..", QString() /* "Cancel" */, 0, t, this);
    calProgress->setWindowFlags(calProgress->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    calProgress->setWindowModality(Qt::WindowModal);
    calProgress->setMinimumWidth(400);
    calProgress->setAutoReset(false);
    calProgress->show();

    calTimer = new QTimer(this);       //声明一个校准计时器
    //延时信号之后，触发 进度条更新的函数：calTimerUpdate
    connect(calTimer, SIGNAL(timeout()), this, SLOT(calTimerUpdate()));
    calTimer->start(500);               // 500毫秒后启动计时器

    calMaxTime = t;
    calTime = 0;
}

void MainWindow::calTimerUpdate()       //校准计时器 （进度条）更新
{
    ++calTime;
    if (calTime > calMaxTime)
    {
            delete calProgress;
            delete calTimer;
    }
    else
    {
        calProgress->setValue(calTime);    //设置进度条的值
    }
}


void MainWindow::FileDatCreate()           //创建文件
{
again:  qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        int randname = qrand();
        fileName = "./imuData"+QString::number(randname,16).toUpper()+".txt";
        QFile file(fileName);
        if(file.exists())
        {
               goto again;
        }
        else
        {
               file.open( QIODevice::ReadWrite | QIODevice::Text );
               file.close();
        }
        QMessageBox::information(NULL, "提示", "文件"+fileName+"已开始记录!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

}

void MainWindow::FileDatLog()              //数据记录（文件写入传感器数据）
{
    QFile file(fileName);

    if(!file.open(QIODevice::Append | QIODevice::Text))
    {
        qDebug()<<"Open failed.";
        return;
    }

    while(!fileIOBuf.empty())
    {
        TypfileIOBuf wirteBuf = fileIOBuf.dequeue();

        QTextStream txtOutput(&file);      //创建一个文件流对象 txtOutput
        QString s1(wirteBuf.curTime+','+
                   //quater[]  eular[] 都是ImuReadThread.cpp 中10毫秒更新的数据，所以此处循环就是每10毫秒的数据
                   QString::number(wirteBuf.quater[0])+','+
                   QString::number(wirteBuf.quater[1])+','+
                   QString::number(wirteBuf.quater[2])+','+
                   QString::number(wirteBuf.quater[3])+','+

                   QString::number(wirteBuf.eular[0])+','+
                   QString::number(wirteBuf.eular[1])+','+
                   QString::number(wirteBuf.eular[2])+','+

                   QString::number(wirteBuf.quater1[0])+','+
                   QString::number(wirteBuf.quater1[1])+','+
                   QString::number(wirteBuf.quater1[2])+','+
                   QString::number(wirteBuf.quater1[3])+','+

                   QString::number(wirteBuf.eular1[0])+','+
                   QString::number(wirteBuf.eular1[1])+','+
                   QString::number(wirteBuf.eular1[2])+';'+


                   QString::number(wirteBuf.qa[0])+','+
                   QString::number(wirteBuf.qa[1])+','+
                   QString::number(wirteBuf.qa[2])+','+
                   QString::number(wirteBuf.qa[3])
                   );

        txtOutput << s1 << endl;


    }

    file.close();

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_threadAButton_clicked()
{
    if(threadA->isRunning())
    {
        threadA->stop();
        ui->threadAButton->setText(tr("打开IMU"));
    }
    else
    {
        threadA->start();                               //启动线程 ImuReadThread
        ui->threadAButton->setText(tr("关闭IMU"));
    }
}

void MainWindow::on_threadBButton_clicked()
{
    if(calibrateFLAG == 0)
    {
        calibrateFLAG++;
        lpms->startMagCalibration();
        startCalWaitBar(45);
        ui->threadBButton->setText("静置IMU1");               //she zi imu1
    }
    else if(calibrateFLAG==1)
    {
        calibrateFLAG++;
        lpms->startMagReferenceCal();
        startSetWaitBar(10);
        ui->threadBButton->setText("上传IMU1");               //

    }
    else if(calibrateFLAG==2)
    {
        calibrateFLAG++;
        lpms->saveCalibrationData();
        ui->threadBButton->setText("校准IMU2");

    }
    else if(calibrateFLAG==3)
    {
        calibrateFLAG++;
        lpms1->startMagCalibration();
        startCalWaitBar(45);
        ui->threadBButton->setText("静置IMU2");

    }
    else if(calibrateFLAG==4)
    {
        calibrateFLAG++;
        lpms1->startMagReferenceCal();
        startSetWaitBar(10);
        ui->threadBButton->setText("上传IMU2");
    }
    else if(calibrateFLAG==5)
    {
        calibrateFLAG=0;
        lpms1->saveCalibrationData();
        ui->threadBButton->setText("校准IMU1");

    }
}

void MainWindow::on_quitButton_clicked()
{
    threadA->stop();
    threadB->stop();
    exit(0);

}

void MainWindow::on_netStart_clicked()
{
    if(nethread->isRunning())
    {
        nethread->stop();
        ui->netStart->setText("连接");
        ui->ipaddrInput->setEnabled(true);
        ui->portInput->setEnabled(true);
    }
    else        //mei pao  (kai qi)
    {
        nethread->start();  //kaiqi  udp
        ui->netStart->setText("断开");    //set btn  disconnect text
        ui->ipaddrInput->setEnabled(false); //grey ip
        ui->portInput->setEnabled(false);
        ipaddr = ui->ipaddrInput->text();
        netport= ui->portInput->text();
    }
}

void MainWindow::on_datRecBtn_clicked()
{
    FileDatCreate();
    recFlag = true;
    ui->datRecBtn->setEnabled(false);
}

void MainWindow::on_offsetButton_clicked()
{
    if(thread1ok != true)
        return;
    lpms->setOrientationOffset(0);
}

void MainWindow::on_angleCalButton_clicked()
{
    if(thread1ok != true)
        return;
    else
    {
        angleCalFLAG = ~angleCalFLAG;
    }
}

void MainWindow::on_offerset1Button_clicked()
{
    if(thread1ok != true)
        return;
    lpms1->setOrientationOffset(0);
}
