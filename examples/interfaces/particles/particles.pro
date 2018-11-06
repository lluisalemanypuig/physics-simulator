TEMPLATE = app
TARGET = particles-renderer

QT += core gui opengl
CONFIG += c++11

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

LIBS += -lGLU

FORMS += mainwindow.ui

SOURCES +=						\
	main.cpp					\
	mainwindow.cpp				\
	simulation_renderer.cpp		\
	render_simulation.cpp		\
	utils.cpp					\
	sim1.cpp					\
	sim2.cpp					\
	sim3.cpp					\
	sim4.cpp					\
	sim5.cpp					\
	sim6.cpp

HEADERS +=						\
    mainwindow.hpp				\
    simulation_renderer.hpp		\
	utils.hpp

RESOURCES +=

# base (mesh, .obj reader, ...)
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../base/release/ -lbase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../base/debug/ -lbase
else:unix: LIBS += -L$$OUT_PWD/../base/ -lbase

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

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
