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
    managedjsondb.cpp

HEADERS  += mainwindow.h \
    managedb.h \
    customfunctions.h \
    managedjsondb.h

FORMS    += mainwindow.ui \
    exercise.ui \
    newword.ui

CONFIG  += mobility
CONFIG += C++11
MOBILITY = 
