#-------------------------------------------------
#
# Project created by QtCreator 2022-01-25T20:35:03
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VendingSystem_server
TEMPLATE = app


SOURCES += main.cpp\
        login.cpp \
    menu.cpp \
    tablemodel.cpp \
    dbhandler.cpp

HEADERS  += login.h \
    menu.h \
    tablemodel.h \
    dbhandler.h

FORMS    += login.ui \
    menu.ui \
    userreg.ui
