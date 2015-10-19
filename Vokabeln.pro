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
    customfunctions.cpp

HEADERS  += mainwindow.h \
    managedb.h \
    customfunctions.h

FORMS    += mainwindow.ui

CONFIG  += c++11
