TEMPLATE = lib

CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += staticlib

QMAKE_CXXFLAGS_DEBUG += -DDEBUG -g
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG
INCLUDEPATH += ..

HEADERS +=							\
	simulator.hpp					\
	geometry/geometry.hpp			\
	geometry/triangle.hpp			\
	geometry/sphere.hpp				\
	geometry/plane.hpp				\
	geometry/rectangle.hpp			\
	initialiser/initialiser.hpp		\
	initialiser/rect_fountain.hpp	\
	initialiser/rect_source.hpp		\
	initialiser/rect_shower.hpp		\
    initialiser/hose.hpp			\
    particles/particle.hpp


SOURCES +=							\
	simulator.cpp					\
	geometry/geometry.cpp			\
	geometry/triangle.cpp			\
	geometry/sphere.cpp				\
	geometry/plane.cpp				\
	geometry/rectangle.cpp			\
	initialiser/initialiser.cpp		\
	initialiser/rect_fountain.cpp	\
	initialiser/rect_source.cpp		\
	initialiser/rect_shower.cpp		\
    initialiser/hose.cpp			\
    particles/particle.cpp


