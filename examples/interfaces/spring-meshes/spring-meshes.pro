TEMPLATE = app
TARGET = spring-meshes

CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

SOURCES += main.cpp \
    glut_functions.cpp \
    study_cases.cpp \
    utils.cpp \
    1_meshes1d.cpp \
    2_mesh2d_reg.cpp \
    3_mesh2d_reg_sphere.cpp

HEADERS += \
    study_cases.hpp \
    glut_functions.hpp \
    utils.hpp

# base (model, obj reader, ...)
LIBS += -L../base/ -lbase
PRE_TARGETDEPS += ../base/libbase.a
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

unix {
	LIBS += -lglut -lGLU
	exists(/usr/lib/nvidia-304/) {
		LIBS += -L/usr/lib/nvidia-304/ -lGL
	}
	else {
		LIBS += -lGL
	}
}
