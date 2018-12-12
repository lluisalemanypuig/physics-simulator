TEMPLATE = app
TARGET = particles

CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

HEADERS += \
    glut_functions.hpp \
    utils.hpp \
    conversion_helper.hpp \
    glut_variables.hpp

SOURCES += main.cpp \
    glut_functions.cpp \
    utils.cpp \
    sim_00.cpp \
    sim_01.cpp \
    sim_02.cpp \
    sim_03.cpp \
    sim_04.cpp \
    sim_05.cpp \
    sim_06.cpp \
    sim_07.cpp \
    sim_08.cpp \
    sim_09.cpp \
    sim_10.cpp \
    glut_variables.cpp \
    glut_render.cpp \
    sim_11.cpp \
    sim_12.cpp

# render (model, obj reader, ...)
LIBS += -L../render/ -lrender
PRE_TARGETDEPS += ../render/librender.a
INCLUDEPATH += ..
DEPENDPATH += ..

# physim library
CONFIG(debug, debug|release) {
    LIBS += -L../../../physim-debug/ -lphysim
    PRE_TARGETDEPS += ../../../physim-debug/libphysim.a
}
CONFIG(release, debug|release) {
    LIBS += -L../../../physim-release/ -lphysim
    PRE_TARGETDEPS += ../../../physim-release/libphysim.a
}
INCLUDEPATH += ../../..
DEPENDPATH += ../../..

LIBS += -lglut -lGLU -lGLEW
unix {
    exists(/usr/lib/nvidia-304/) {
        LIBS += -L/usr/lib/nvidia-304/ -lGL
    }
    else {
        LIBS += -lGL
    }
}
