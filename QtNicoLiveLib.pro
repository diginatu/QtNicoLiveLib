#-------------------------------------------------
#
# Project created by QtCreator 2015-10-18T21:20:57
#
#-------------------------------------------------

QT       += network
QT       += sql
QT       -= gui

CONFIG += c++11

# libs used in nicookie
win32: LIBS += -lwininet
win32-msvc*{
LIBS += -liepmapi
}
win32: LIBS += -lcrypt32
mac: LIBS += -framework Security
unix: LIBS += -lcrypto


TARGET = QtNicoLiveLib
TEMPLATE = lib

DEFINES += QTNICOLIVELIB_LIBRARY

SOURCES += nl/fetchusername.cpp \
    nl/httpgetter.cpp \
    nl/qtnicolivelib.cpp \
    nl/strabstractor.cpp \
    nl/getcommpostkey.cpp \
    nl/livewaku.cpp \
    nl/getplayerstatus.cpp \
    nl/commentconnection.cpp \
    nl/extendinfo.cpp \
    nl/extend.cpp \
    nl/livetag.cpp \
    nl/usersessionlogin.cpp \
    nl/alertlogin.cpp \
    nl/alertadmin.cpp \
    nl/alertconnection.cpp \
    nl/nicookie.cpp

HEADERS += nl/fetchusername.h \
    nl/httpgetter.h \
    nl/qtnicolivelib.h \
    nl/qtnicolivelib_global.h \
    nl/strabstractor.h \
    nl/getcommpostkey.h \
    nl/livewaku.h \
    nl/getplayerstatus.h \
    nl/commentconnection.h \
    nl/extendinfo.h \
    nl/extend.h \
    nl/livetag.h \
    nl/usersessionlogin.h \
    nl/alertlogin.h \
    nl/alertadmin.h \
    nl/alertconnection.h \
    nl/nicookie.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
