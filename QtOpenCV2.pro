#-------------------------------------------------
#
# Project created by QtCreator 2013-05-18T01:56:28
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtOpenCV2
TEMPLATE = app

unix{
LIBS += `pkg-config opencv --cflags --libs`
}

CONFIG += debug_and_release
win32:debug {
     CONFIG += console
INCLUDEPATH +=C:\opencv-2.4.5\opencv\build\include

LIBS += -LC:\\opencv-2.4.5\\opencv\\build\\x86\\vc10\\lib -lopencv_core245d -lopencv_highgui245d -lopencv_imgproc245d

 }

win32:release {
INCLUDEPATH +=C:\opencv-2.4.5\opencv\build\include
LIBS += -LC:\\opencv-2.4.5\\opencv\\build\\x86\\vc10\\lib -lopencv_core245 -lopencv_highgui245 -lopencv_imgproc245

}

SOURCES += main.cpp\
        mainwindow.cpp \
    systemsettings.cpp \
    exception.cpp \
    cvs_uavtargetdetectionapp.cpp \
    imageretrieve.cpp \
    videoretrieve.cpp \
    cameraretrieve.cpp \
    imageprocess.cpp \
    cvutility.cpp

HEADERS  += mainwindow.h \
    systemsettings.h \
    exception.h \
    cvs_uavtargetdetectionapp.h \
    imageretrieve.h \
    videoretrieve.h \
    cameraretrieve.h \
    imageprocess.h \
    cvutility.h

FORMS    += mainwindow.ui
