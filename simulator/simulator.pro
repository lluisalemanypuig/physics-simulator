TEMPLATE = lib

CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += staticlib

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG
INCLUDEPATH += ..

HEADERS +=									\
	simulator.hpp							\
	particle.hpp							\
	geometry/geometry.hpp					\
	geometry/triangle.hpp					\
	geometry/sphere.hpp						\
	geometry/plane.hpp						\
	initialiser/initialiser.hpp				\
	initialiser/rectangular_source.hpp		\
	initialiser/rect_waterfall.hpp			\
    initialiser/rect_fountain.hpp


SOURCES +=									\
	simulator.cpp							\
	particle.cpp							\
	geometry/geometry.cpp					\
	geometry/triangle.cpp					\
	geometry/sphere.cpp						\
	geometry/plane.cpp						\
	initialiser/initialiser.cpp				\
	initialiser/rectangular_source.cpp		\
	initialiser/rect_waterfall.cpp			\
    initialiser/rect_fountain.cpp


