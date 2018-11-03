TEMPLATE = app
TARGET = basic-viewer

CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

SOURCES += main.cpp

# base (model, obj reader, ...)
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../base/release/ -lbase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../base/debug/ -lbase
else:unix: LIBS += -L$$OUT_PWD/../base/ -lbase

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../base/release/libbase.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../base/debug/libbase.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../base/release/base.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../base/debug/base.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../base/libbase.a


# physim library
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../physim-release/ -lphysim
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../physim-debug/ -lphysim
else:unix: LIBS += -L$$PWD/../../../physim-debug/ -lphysim

INCLUDEPATH += $$PWD/../../../
DEPENDPATH += $$PWD/../../../

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../physim-release/libphysim.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../physim-debug/libphysim.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../physim-release/physim.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../physim-debug/physim.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../physim-debug/libphysim.a

unix {
	LIBS += -lglut -lGLU
	exists(/usr/lib/nvidia-304/) {
		LIBS += -L/usr/lib/nvidia-304/ -lGL
	}
	else {
		LIBS += -lGL
	}
}
