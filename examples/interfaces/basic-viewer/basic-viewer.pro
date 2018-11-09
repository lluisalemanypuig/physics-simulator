TEMPLATE = app
TARGET = basic-viewer

CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

SOURCES += main.cpp \
    utils.cpp

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

HEADERS += \
    utils.hpp
