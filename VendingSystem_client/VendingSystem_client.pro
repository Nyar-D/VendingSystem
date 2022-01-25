#-------------------------------------------------
#
# Project created by QtCreator 2022-01-25T05:19:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VendingSystem_client
TEMPLATE = app


SOURCES += main.cpp\
        advertise.cpp \
    shop.cpp

HEADERS  += advertise.h \
    shop.h

FORMS    += advertise.ui \
    shop.ui

RESOURCES += \
    advertisement.qrc
