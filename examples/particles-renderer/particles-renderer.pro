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

LIBS += -lGL -lGLU

FORMS += mainwindow.ui

SOURCES +=					\
	main.cpp				\
	mainwindow.cpp			\
	simulation_renderer.cpp	\
    render_simulation.cpp	\
    sim0.cpp				\
    sim1.cpp				\
    sim2.cpp				\
    sim3.cpp				\
    sim4.cpp				\
    obj_reader.cpp			\
    mesh.cpp				\
    rgeom.cpp				\
    rplane.cpp				\
    rtriangle.cpp			\
    rrectangle.cpp			\
    rsphere.cpp				\
    utils.cpp \
    sim5.cpp

HEADERS +=					\
    mainwindow.hpp			\
    simulation_renderer.hpp \
    obj_reader.hpp			\
    mesh.hpp				\
    render_geometry.hpp		\
    utils.hpp

DISTFILES +=					\
	shaders/simpleshader.vert	\
    shaders/simpleshader.frag

RESOURCES += \
    resources.qrc
