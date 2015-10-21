#-------------------------------------------------
#
# Project created by QtCreator 2015-08-23T10:52:05
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Vokabeln
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    managedb.cpp \
    customfunctions.cpp \
    selectpath.cpp

HEADERS  += mainwindow.h \
    managedb.h \
    customfunctions.h \
    selectpath.h

FORMS    += mainwindow.ui \
    selectPath.ui

CONFIG  += c++11
