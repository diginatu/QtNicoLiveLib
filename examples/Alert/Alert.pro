QT += core
QT += network
QT -= gui

TARGET = Alert
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    testclass.cpp

HEADERS += \
    testclass.h

LIBS += -L$$PWD/../../bin/ -lQtNicoLiveLib

INCLUDEPATH += $$PWD/../../
DEPENDPATH += $$PWD/../../
