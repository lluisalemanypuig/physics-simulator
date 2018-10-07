TEMPLATE = app
TARGET = particles-renderer

QT += core gui opengl
CONFIG += c++11

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

INCLUDEPATH += . ../..

CONFIG(release, debug|release) {
	LIBS += -L../../simulator-release/ -lsimulator
}
CONFIG(debug, debug|release) {
	LIBS += -L../../simulator-debug/ -lsimulator
}

FORMS += mainwindow.ui

SOURCES +=					\
	main.cpp				\
	mainwindow.cpp			\
	simulation_renderer.cpp	\
	utils.cpp				\
	scene_0.cpp				\
	scene_lab.cpp			\
    render_simulation.cpp

HEADERS +=					\
    simulation_renderer.hpp \
	utils.hpp				\
	mainwindow.hpp			\
    render_geometry.hpp

DISTFILES +=					\
	shaders/simpleshader.vert	\
    shaders/simpleshader.frag

RESOURCES += \
    resources.qrc
