#-------------------------------------------------
#
# Project created by QtCreator 2013-05-18T01:56:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtOpenCV2
TEMPLATE = app

unix{
LIBS += `pkg-config opencv --cflags --libs`
}

CONFIG += debug_and_release
win32:debug {
     CONFIG += console
INCLUDEPATH +=D:/OpenCv-2.4.6/opencv/build/include
INCLUDEPATH +=D:/cvs/qtopencv

LIBS += -LD:/OpenCv-2.4.6/opencv/build/x86/mingw/lib -lopencv_core246.dll -lopencv_highgui246.dll -lopencv_imgproc246.dll -lopencv_features2d246.dll -lopencv_video246.dll -lopencv_nonfree246.dll -lopencv_calib3d246.dll

 }

win32:release {
INCLUDEPATH +=D:/OpenCv-2.4.6/opencv/build/include
INCLUDEPATH +=D:/cvs/qtopencv
LIBS += -LD:/OpenCv-2.4.6/opencv/build/x86/mingw/lib -lopencv_core246 -lopencv_highgui246 -lopencv_imgproc246 -lopencv_features2d246 -lopencv_video246 -lopencv_nonfree246 -lopencv_calib3d246


}

SOURCES += main.cpp\
            mainwindow.cpp \
            exception.cpp \
            cvs_uavtargetdetectionapp.cpp \
            imageretrieve.cpp \
            videoretrieve.cpp \
            cameraretrieve.cpp \
            imageprocess.cpp \
    framealignment.cpp \
    framealigner.cpp \
    candidatefilter.cpp \
    candidatedetector.cpp \
    alignmentmatrixcalc.cpp \
    alarmgenerator.cpp \
    preprocess.cpp \
    Test/TEST_frameAllignment.cpp\
    movevector.cpp \
    dialog.cpp \
    frameproducer.cpp \
    frameconsumer.cpp

HEADERS  += mainwindow.h \
            exception.h \
            cvs_uavtargetdetectionapp.h \
            imageretrieve.h \
            videoretrieve.h \
            cameraretrieve.h \
            imageprocess.h \
    framealignment.h \
    framealigner.h \
    candidatefilter.h \
    candidatedetector.h \
    alarmgenerator.h \
    alignmentmatrixcalc.h \
    preprocess.h \
    Test/TEST_frameAllignment.h \
    movevector.h \
    dialog.h \
    frameproducer.h \
    frameconsumer.h

FORMS    += mainwindow.ui \
    dialog.ui
