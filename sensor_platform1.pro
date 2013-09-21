#-------------------------------------------------
#
# Project created by QtCreator 2013-09-01T20:35:30
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sensor_platform1
TEMPLATE = app


SOURCES += main.cpp\
        sensorwidget.cpp \
    node.cpp \
    sqlhelper.cpp \
    maplabel.cpp \
    infodialog.cpp \
    request.cpp \
    plotdialog.cpp \
    qcustomplot.cpp

HEADERS  += sensorwidget.h \
    node.h \
    sqlhelper.h \
    maplabel.h \
    infodialog.h \
    request.h \
    plotdialog.h \
    qcustomplot.h

FORMS    += sensorwidget.ui \
    infodialog.ui \
    plotdialog.ui

RESOURCES += \
    resource.qrc
