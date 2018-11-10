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
