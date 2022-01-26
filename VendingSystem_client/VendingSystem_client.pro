#-------------------------------------------------
#
# Project created by QtCreator 2022-01-25T20:09:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VendingSystem_client
TEMPLATE = app


SOURCES += main.cpp\
        advertisement.cpp \
    shop.cpp

HEADERS  += advertisement.h \
    shop.h

FORMS    += advertisement.ui \
    shop.ui

RESOURCES += \
    media.qrc
