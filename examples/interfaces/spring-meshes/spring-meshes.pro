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
    0_meshes1d.cpp

HEADERS +=				\
	study_cases.hpp		\
	glut_functions.hpp	\
    utils.hpp

# base (model, obj reader, ...)
CONFIG(release, debug|release): LIBS += -L../../base-release/ -lbase
CONFIG(debug, debug|release): LIBS += -L../../base-debug/ -lbase

INCLUDEPATH += ..
DEPENDPATH += ..

# physim library
CONFIG(release, debug|release): LIBS += -L../../physim-release/ -lphysim
CONFIG(debug, debug|release): LIBS += -L../../physim-debug/ -lphysim

INCLUDEPATH += ../..
DEPENDPATH += ../..

unix {
	LIBS += -lglut -lGLU
	exists(/usr/lib/nvidia-304/) {
		LIBS += -L/usr/lib/nvidia-304/ -lGL
	}
	else {
		LIBS += -lGL
	}
}
