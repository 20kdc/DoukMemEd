#-------------------------------------------------
#
# Project created by QtCreator 2018-10-01T09:15:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DoukMemEd
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        doukmemed.cpp \
        views/doukview.cpp \
        views/doukmapview.cpp \
    views/doukstatusview.cpp \
    views/douknpcview.cpp

win32: SOURCES += lpa/lpa_windows.cpp
else: SOURCES += lpa/lpa_unix.cpp

HEADERS += \
        doukmemed.h \
        doukutsu.h \
        lpa/lpa.h \
        views/doukview.h \
        views/doukmapview.h \
    views/doukstatusview.h \
    views/douknpcview.h

FORMS += \
        doukmemed.ui \
    views/doukstatusview.ui \
    views/douknpcview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -lpsapi
# Just in case.
win32: DEFINES += PSAPI_VERSION=1

data.path = $$OUT_PWD
data.files = \
    data/*

INSTALLS += \
    data
