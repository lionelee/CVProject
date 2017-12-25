#-------------------------------------------------
#
# Project created by QtCreator 2017-12-03T22:18:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CVImgProcessor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


HEADERS += \
        imgwidget.h \
        imgprocess.h \
        mainwindow.h \
        toolbarinfo.h \
        toolbartool.h \
        binarydialog.h \
        hsldialog.h \
        histogramwidget.h \
    toolbaropt.h \
    cutmodal.h


SOURCES += \
        imgwidget.cpp \
        imgprocess.cpp \
        mainwindow.cpp \
        main.cpp \
        toolbarinfo.cpp \
        toolbartool.cpp \
        hsldialog.cpp \
        binarydialog.cpp \
        histogramwidget.cpp \
    toolbaropt.cpp \
    cutmodal.cpp

FORMS += \
        mainwindow.ui

INCLUDEPATH += C:\opencv\include\opencv \
                C:\opencv\include\opencv2 \
                C:\opencv\include

LIBS += -LC:\opencv\lib \
-lopencv_world331d

RESOURCES += \
    res.qrc

RC_FILE += \
    app.rc
