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
	0_1dmeshes.cpp		\
	study_cases.cpp		\
    utils.cpp

HEADERS +=				\
	study_cases.hpp		\
	glut_functions.hpp	\
    utils.hpp

# base (model, obj reader, ...)
win32:CONFIG(release, debug|release): LIBS += -L../base/release/ -lbase
else:win32:CONFIG(debug, debug|release): LIBS += -L../base/debug/ -lbase
else:unix: LIBS += -L../base/ -lbase

INCLUDEPATH += ..
DEPENDPATH += ..

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += ../base/release/libbase.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += ../base/debug/libbase.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += ../base/release/base.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += ../base/debug/base.lib
else:unix: PRE_TARGETDEPS += ../base/libbase.a

# physim library
win32:CONFIG(release, debug|release): LIBS += -L../../../physim-release/ -lphysim
else:win32:CONFIG(debug, debug|release): LIBS += -L../../../physim-debug/ -lphysim
else:unix: LIBS += -L../../../physim-debug/ -lphysim

INCLUDEPATH += ../../..
DEPENDPATH += ../../..

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += ../../../physim-release/libphysim.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += ../../../physim-debug/libphysim.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += ../../../physim-release/physim.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += ../../../physim-debug/physim.lib
else:unix: PRE_TARGETDEPS += ../../../physim-debug/libphysim.a

unix {
	LIBS += -lglut -lGLU
	exists(/usr/lib/nvidia-304/) {
		LIBS += -L/usr/lib/nvidia-304/ -lGL
	}
	else {
		LIBS += -lGL
	}
}
