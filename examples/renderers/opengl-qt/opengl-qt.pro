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

HEADERS +=			\
	mainwindow.h	\
	renderer.hpp

FORMS    += mainwindow.ui

# mesh and .obj reader
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../opengl/release/ -lopengl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../opengl/debug/ -lopengl
else:unix: LIBS += -L$$OUT_PWD/../opengl/ -lopengl

INCLUDEPATH += $$PWD/../opengl
DEPENDPATH += $$PWD/../opengl

# physim library
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../physim-release/ -lphysim
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../physim-debug/ -lphysim
else:unix: LIBS += -L$$PWD/../../../physim-debug/ -lphysim

INCLUDEPATH += $$PWD/../../../physim
DEPENDPATH += $$PWD/../../../physim

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../physim-release/libphysim.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../physim-debug/libphysim.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../physim-release/physim.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../physim-debug/physim.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../physim-debug/libphysim.a
