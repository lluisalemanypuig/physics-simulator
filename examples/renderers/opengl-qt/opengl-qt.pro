#-------------------------------------------------
#
# Project created by QtCreator 2018-10-11T21:23:43
#
#-------------------------------------------------

QT		+= core gui opengl
CONFIG	+= c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opengl-qt
TEMPLATE = app

LIBS += -lGLU

QMAKE_CXXFLAGS_DEBUG += -DDEBUG -g
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

SOURCES += main.cpp	\
	mainwindow.cpp	\
	renderer.cpp

HEADERS +=				\
	mainwindow.h		\
	renderer.hpp

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../opengl/release/ -lopengl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../opengl/debug/ -lopengl
else:unix: LIBS += -L$$OUT_PWD/../opengl/ -lopengl

INCLUDEPATH += $$PWD/../opengl
DEPENDPATH += $$PWD/../opengl
