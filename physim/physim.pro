TEMPLATE = lib
TARGET = physim

CONFIG += c++11
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DDEBUG -g
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG
INCLUDEPATH += ..

HEADERS +=							\
	simulator.hpp					\
	sim_solver.cpp					\
	particles/free_particle.hpp		\
	particles/mesh_particle.hpp		\
	meshes/mesh1d.hpp				\
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
    fields/field.hpp				\
    fields/punctual.hpp				\
    fields/magnetic.hpp				\
    fields/magnetic_B.hpp			\
    fields/electric.hpp				\
    fields/gravitational.hpp		\
	fields/gravitational_planet.hpp	\
	math/vec3.hpp					\
	math/math.hpp					\
	math/math_ops.hpp				\
	math/math_ops_add.hpp			\
	math/math_ops_sub.hpp			\
	math/math_ops_mul.hpp			\
	math/math_ops_div.hpp			\
	math/math_ops_mixed.hpp			\
	math/math_misc.hpp				\
	math/math_geometry.hpp

SOURCES +=							\
	simulator.cpp					\
	sim_free_particles.cpp			\
	sim_meshes1d.cpp				\
	particles/free_particle.cpp		\
	particles/mesh_particle.cpp		\
	meshes/mesh1d.cpp				\
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
    fields/field.cpp				\
    fields/electric.cpp				\
    fields/magnetic_B.cpp			\
    fields/magnetic.cpp				\
    fields/punctual.cpp				\
    fields/gravitational.cpp		\
	fields/gravitational_planet.cpp


