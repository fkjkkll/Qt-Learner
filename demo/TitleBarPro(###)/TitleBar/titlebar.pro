#-------------------------------------------------
#
# Project created by QtCreator 2021-04-29T09:42:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyTitleBar
TEMPLATE = app


SOURCES += main.cpp \
    mainwidget.cpp \
    titlebar.cpp

HEADERS  += \
    mainwidget.h \
    titlebar.h

FORMS    += \
    titlebar.ui \
    mainwidget.ui

RESOURCES += \
    resource/img.qrc

RC_ICONS = food.ico

