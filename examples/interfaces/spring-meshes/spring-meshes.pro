TEMPLATE = app
TARGET = spring-meshes

CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

SOURCES += main.cpp		\
	glut_functions.cpp	\
	study_cases.cpp		\
    utils.cpp \
    0_meshes1d.cpp \
    1_meshes2d.cpp

HEADERS +=				\
	study_cases.hpp		\
	glut_functions.hpp	\
    utils.hpp

# base (model, obj reader, ...)
CONFIG(debug, debug|release): LIBS += -L../base/ -lbase
CONFIG(release, debug|release): LIBS += -L../base/ -lbase

INCLUDEPATH += ..
DEPENDPATH += ..

CONFIG(debug, debug|release): PRE_TARGETDEPS += ../base/libbase.a
CONFIG(release, debug|release): PRE_TARGETDEPS += ../base/libbase.a

# physim library
CONFIG(debug, debug|release): LIBS += -L../../../physim-debug/ -lphysim
CONFIG(release, debug|release): LIBS += -L../../../physim-release/ -lphysim

INCLUDEPATH += ../../..
DEPENDPATH += ../../..

CONFIG(debug, debug|release): PRE_TARGETDEPS += ../../../physim-debug/libphysim.a
CONFIG(release, debug|release): PRE_TARGETDEPS += ../../../physim-release/libphysim.a

unix {
	LIBS += -lglut -lGLU
	exists(/usr/lib/nvidia-304/) {
		LIBS += -L/usr/lib/nvidia-304/ -lGL
	}
	else {
		LIBS += -lGL
	}
}
