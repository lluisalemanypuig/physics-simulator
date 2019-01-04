TEMPLATE = app
CONFIG += console c++11
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
    conversion_helper.hpp \
    glut_functions.hpp \
    utils.hpp \
    glut_variables.hpp \
    kernels.hpp

SOURCES += \
    main.cpp \
    glut_functions.cpp \
    glut_render.cpp \
    sim_00.cpp \
    utils.cpp \
    glut_variables.cpp \
    kernels.cpp
