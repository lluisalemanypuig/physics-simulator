TEMPLATE = lib
TARGET = physim

CONFIG += c++11
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG
INCLUDEPATH += ..

HEADERS += \
    simulator.hpp \
    sim_solver.cpp \
    particles/free_particle.hpp \
    particles/mesh_particle.hpp	\
    particles/sized_particle.hpp \
    particles/conversions.hpp \
    geometry/geometry.hpp \
    geometry/triangle.hpp \
    geometry/sphere.hpp \
    geometry/plane.hpp \
    geometry/rectangle.hpp \
    initialiser/initialiser.hpp \
    initialiser/rect_fountain.hpp \
    initialiser/rect_source.hpp \
    initialiser/rect_shower.hpp \
    initialiser/hose.hpp \
    initialiser/multisource.hpp \
    initialiser/multisource.cpp \
    fields/field.hpp \
    fields/punctual.hpp \
    fields/magnetic.hpp \
    fields/magnetic_B.hpp \
    fields/gravitational.hpp \
    fields/gravitational_planet.hpp \
    meshes/mesh.hpp \
    meshes/mesh1d.hpp \
    meshes/mesh2d.hpp \
    math/math.hpp \
    math/vec2.hpp \
    math/vec3.hpp \
    math/vec4.hpp \
    math/vec6.hpp \
    meshes/mesh2d_regular.hpp \
    math/private/math2/div.hpp \
    math/private/math2/add.hpp \
    math/private/math2/sub.hpp \
    math/private/math2/mul.hpp \
    math/private/math2/mixed.hpp \
    math/private/math2/base.hpp \
    math/private/math3/div.hpp \
    math/private/math3/add.hpp \
    math/private/math3/sub.hpp \
    math/private/math3/mul.hpp \
    math/private/math3/mixed.hpp \
    math/private/math3/base.hpp \
    math/private/math_ops.hpp \
    math/private/math2.hpp \
    math/private/math3.hpp \
    math/private/math3/geometry.hpp \
    math/private/math2/geometry.hpp \
    particles/base_particle.hpp \
    particles/particle_types.hpp \
    input/obj_reader.hpp \
    input/ply_reader.hpp \
    geometry/object.hpp \
    input/input.hpp \
    input/soup_reader.hpp

SOURCES += \
    simulator.cpp \
    sim_free_particles.cpp \
    sim_meshes.cpp \
    sim_sized_particles.cpp \
    sim_find_collisions.cpp \
    particles/free_particle.cpp \
    particles/mesh_particle.cpp \
    particles/conversions.cpp \
    particles/sized_particle.cpp \
    geometry/geometry.cpp \
    geometry/triangle.cpp \
    geometry/sphere.cpp \
    geometry/plane.cpp \
    geometry/rectangle.cpp \
    initialiser/initialiser.cpp \
    initialiser/rect_fountain.cpp \
    initialiser/rect_source.cpp \
    initialiser/rect_shower.cpp \
    initialiser/hose.cpp \
    fields/field.cpp \
    fields/magnetic_B.cpp \
    fields/magnetic.cpp \
    fields/punctual.cpp \
    fields/gravitational.cpp \
    fields/gravitational_planet.cpp \
    meshes/mesh.cpp \
    meshes/mesh1d.cpp \
    meshes/mesh2d.cpp \
    meshes/mesh2d_regular.cpp \
    particles/base_particle.cpp \
    input/obj_reader.cpp \
    input/ply_reader.cpp \
    geometry/object.cpp \
    input/input.cpp \
    input/soup_reader.cpp
