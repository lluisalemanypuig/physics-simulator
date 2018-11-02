#include <physim/meshes/mesh1d.hpp>

// C includes
#include <stdlib.h>

namespace physim {
using namespace particles;

namespace meshes {

// PUBLIC

mesh1d::mesh1d() {
	ps = nullptr;
}
mesh1d::~mesh1d() {
	clear();
}

// OPERATORS

mesh1d& mesh1d::operator= (const mesh1d& m) {
	clear();

	N = m.N;
	ps = (mesh_particle **)malloc(N*sizeof(mesh_particle));
	for (size_t i = 0; i < N; ++i) {
		ps[i] = new mesh_particle(*m.ps[i]);
	}

	Ke = m.Ke;
	Kd = m.Kd;
	return *this;
}

// MODIFIERS

void mesh1d::allocate(size_t n) {
	if (ps != nullptr) {
		clear();
	}

	N = n;
	ps = (mesh_particle **)malloc(N*sizeof(mesh_particle *));

	for (size_t i = 0; i < N; ++i) {
		ps[i] = new mesh_particle();
		ps[i]->index = i;
	}
}

void mesh1d::clear() {
	if (ps == nullptr) {
		return;
	}

	mesh_particle *p = ps[0];
	for (size_t i = 0; i < N; ++i, ++p) {
		delete p;
	}
	free(ps);
	ps = nullptr;
}

// SETTERS

void mesh1d::set_elasticity(float ke) {
	Ke = ke;
}

void mesh1d::set_damping(float kd) {
	Kd = kd;
}

// GETTERS

float mesh1d::get_elasticity() const {
	return Ke;
}

float mesh1d::get_damping() const {
	return Kd;
}

size_t mesh1d::size() const {
	return N;
}

mesh_particle **mesh1d::get_particles() {
	return ps;
}

} // -- namespace meshes
} // -- namespace physim
