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
    dbhandler.cpp \
    table.cpp

HEADERS  += login.h \
    menu.h \
    dbhandler.h \
    table.h
    dbhandler.h

FORMS    += login.ui \
    menu.ui \
    user_edit.ui \
    user_reg.ui \
    good_add.ui \
    adv_reg.ui \
    good_edit.ui

RESOURCES += \
    media.qrc
