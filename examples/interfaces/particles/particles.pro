TEMPLATE = app
TARGET = particles

CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

# OpenMP
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

# PNG library
LIBS += -lpng

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

# OpenGL
LIBS += -lglut -lGLU -lGLEW
unix {
    exists(/usr/lib/nvidia-304/) {
        LIBS += -L/usr/lib/nvidia-304/ -lGL
    }
    else {
        LIBS += -lGL
    }
}

# Files
HEADERS += \
    glut_functions.hpp \
    utils.hpp \
    conversion_helper.hpp \
    glut_variables.hpp

SOURCES += main.cpp \
    glut_functions.cpp \
    utils.cpp \
    glut_variables.cpp \
    glut_render.cpp \
    sim_000.cpp \
    sim_001.cpp \
    sim_002.cpp \
    sim_003.cpp \
    sim_004.cpp \
    sim_005.cpp \
    sim_100.cpp \
    sim_101.cpp \
    sim_102.cpp \
    sim_103.cpp \
    sim_104.cpp \
    sim_105.cpp \
    sim_106.cpp \
    sim_006.cpp \
    sim_200.cpp \
    sim_201.cpp \
    sim_107.cpp
