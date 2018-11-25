TEMPLATE = app
TARGET = spring-meshes

CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

HEADERS += \
    study_cases.hpp \
    glut_functions.hpp \
    utils.hpp \
    conversion_helper.hpp

SOURCES += main.cpp \
    glut_functions.cpp \
    study_cases.cpp \
    utils.cpp \
    1_meshes1d.cpp \
    2_mesh2d_reg.cpp \
    3_mesh2d_reg_sphere.cpp

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
