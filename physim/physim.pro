TEMPLATE = lib
TARGET = physim

CONFIG += c++11
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG
INCLUDEPATH += ..

HEADERS +=							\
	simulator.hpp					\
	sim_solver.cpp					\
	particles/free_particle.hpp		\
	particles/mesh_particle.hpp		\
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
    initialiser/multisource.hpp		\
    initialiser/multisource.cpp		\
    fields/field.hpp				\
    fields/punctual.hpp				\
    fields/magnetic.hpp				\
    fields/magnetic_B.hpp			\
    fields/gravitational.hpp		\
	fields/gravitational_planet.hpp	\
	math/math.hpp					\
	math/vec2.hpp					\
	math/vec3.hpp					\
	math/vec4.hpp					\
	math/math_ops.hpp				\
	math/math_ops_add.hpp			\
	math/math_ops_sub.hpp			\
	math/math_ops_mul.hpp			\
	math/math_ops_div.hpp			\
	math/math_ops_mixed.hpp			\
	math/math_misc.hpp				\
	math/math_geometry.hpp			\
	math/math_private.hpp			\
	meshes/mesh.hpp					\
    meshes/mesh1d.hp				\
    particles/conversions.hpp

SOURCES +=							\
	simulator.cpp					\
	sim_free_particles.cpp			\
	sim_meshes.cpp					\
	particles/free_particle.cpp		\
	particles/mesh_particle.cpp		\
    particles/conversions.cpp		\
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
    fields/magnetic_B.cpp			\
    fields/magnetic.cpp				\
    fields/punctual.cpp				\
    fields/gravitational.cpp		\
	fields/gravitational_planet.cpp \
	meshes/mesh.cpp					\
    meshes/mesh1d.cpp
