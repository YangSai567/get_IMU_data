#-------------------------------------------------
#
# Project created by QtCreator 2019-01-06T20:54:16
#
#-------------------------------------------------

QT       += core gui\
            opengl\
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sensordisp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    IMUReadThread.cpp \
    IMUReadThread1.cpp \
    myopengl.cpp \
    myopengl1.cpp \
    UdpTxThread.cpp \
    uplimbdispthread.cpp

HEADERS  += mainwindow.h \
    IMUReadThread.h \
    IMUReadThread1.h \
    myopengl.h \
    myopengl1.h \
    UdpTxThread.h \
    uplimbdispthread.h \
    ../../../../../usr/include/lpsensor/LpmsSensorI.h \
    ../../../../../usr/include/lpsensor/LpmsSensorManagerI.h

FORMS    += mainwindow.ui


LIBS += -lglut -lGLU
LIBS += -L/home/wam/Desktop/sensordisp/ -lLpSensor
#LIBS += -L/home/wam/Desktop/sensordisp/ -lLpSensorD


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/release/ -lLpSensor
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/debug/ -lLpSensor
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/ -lLpSensor

INCLUDEPATH += $$PWD/../../../../../usr/include
INCLUDEPATH += /usr/local/include/
INCLUDEPATH += /usr/include/
INCLUDEPATH += /home/wam/Desktop/sensordisp/sensordisp/inc/

INCLUDEPATH += /usr/local/freetype/include/
INCLUDEPATH += /usr/local/freetype/include/freetype2/
DEPENDPATH += $$PWD/../../../../../usr/include

unix|win32: LIBS += -lfreetype

unix|win32: LIBS += -lglut

RC_FILE += sensordisp.rc

