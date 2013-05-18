#-------------------------------------------------
#
# Project created by QtCreator 2013-05-18T01:56:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtOpenCV2
TEMPLATE = app

LIBS += `pkg-config opencv --cflags --libs`


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
