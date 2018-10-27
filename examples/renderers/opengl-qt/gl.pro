#-------------------------------------------------
#
# Project created by QtCreator 2018-10-27T23:33:21
#
#-------------------------------------------------

QT       += opengl

QT       -= core gui

TARGET = gl
TEMPLATE = lib

DEFINES += GL_LIBRARY

SOURCES += genericplugin.cpp

HEADERS += genericplugin.h\
        gl_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
