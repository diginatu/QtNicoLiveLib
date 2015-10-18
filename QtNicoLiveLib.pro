#-------------------------------------------------
#
# Project created by QtCreator 2015-10-18T21:20:57
#
#-------------------------------------------------

QT       += network
QT       -= gui

CONFIG += c++11

TARGET = QtNicoLiveLib
TEMPLATE = lib

DEFINES += QTNICOLIVELIB_LIBRARY

SOURCES += qtnicolivelib.cpp \
    httpgetter.cpp \
    fetchusername.cpp \
    strabstractor.cpp

HEADERS += qtnicolivelib.h\
        qtnicolivelib_global.h \
    httpgetter.h \
    fetchusername.h \
    strabstractor.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
