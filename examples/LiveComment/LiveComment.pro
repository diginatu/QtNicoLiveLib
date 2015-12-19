QT += core
QT += network
QT -= gui

TARGET = LiveComment
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    testclass.cpp

HEADERS += \
    testclass.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../build-QtNicoLiveLib-Desktop_Qt_5_5_0_GCC_64bit-Release/release/ -lQtNicoLiveLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../build-QtNicoLiveLib-Desktop_Qt_5_5_0_GCC_64bit-Release/debug/ -lQtNicoLiveLib
else:unix: LIBS += -L$$PWD/../../bin/linux/ -lQtNicoLiveLib

INCLUDEPATH += $$PWD/../../
DEPENDPATH += $$PWD/../../
