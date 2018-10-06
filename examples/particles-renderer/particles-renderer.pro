TEMPLATE = app
TARGET = particles-renderer

QT += core gui opengl
CONFIG  += c++11

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

INCLUDEPATH += .

FORMS += mainwindow.ui

SOURCES +=          \
    main.cpp        \
    mainwindow.cpp \
    simulation_renderer.cpp

HEADERS +=          \
    mainwindow.h \
    simulation_renderer.hpp

DISTFILES += \
    shaders/simpleshader.vert \
    shaders/simpleshader.frag

RESOURCES += \
    resources.qrc
