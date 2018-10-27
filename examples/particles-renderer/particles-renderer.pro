TEMPLATE = app
TARGET = particles-renderer

QT += core gui opengl
CONFIG += c++11

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

INCLUDEPATH += . ../..

CONFIG(release, debug|release) {
	LIBS += -L../../physim-release/ -lphysim
}
CONFIG(debug, debug|release) {
	LIBS += -L../../physim-debug/ -lphysim
}

LIBS += -lGL -lGLU

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
	sim6.cpp					\
	mesh/obj_reader.cpp			\
	mesh/mesh.cpp				\
	rgeom/rgeom.cpp				\
	rgeom/rplane.cpp			\
	rgeom/rtriangle.cpp			\
	rgeom/rrectangle.cpp		\
	rgeom/rsphere.cpp

HEADERS +=						\
    mainwindow.hpp				\
    simulation_renderer.hpp		\
    utils.hpp					\
    mesh/obj_reader.hpp			\
	mesh/mesh.hpp				\
    rgeom/rendered_geometry.hpp

DISTFILES +=					\
	shaders/simpleshader.vert	\
    shaders/simpleshader.frag

RESOURCES += \
    resources.qrc
