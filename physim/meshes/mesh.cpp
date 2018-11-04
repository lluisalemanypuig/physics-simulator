#include <physim/meshes/mesh.hpp>

// C includes
#include <assert.h>
#include <stdlib.h>

namespace physim {
using namespace particles;

namespace meshes {

// PUBLIC

mesh::mesh() {
	friction = 0.0f;
	bouncing = 0.8f;
	Ke = 0.0f;
	Kd = 0.0f;
	N = 0;
	ps = nullptr;
}
mesh::mesh(float ke, float kd) {
	friction = 0.0f;
	bouncing = 0.8f;
	Ke = ke;
	Kd = kd;
	N = 0;
	ps = nullptr;
}

mesh::~mesh() {
	clear();
}

// OPERATORS

particles::mesh_particle *mesh::operator[] (size_t i) {
	assert(i < N);
	return ps[i];
}

const particles::mesh_particle *mesh::operator[] (size_t i) const {
	assert(i < N);
	return ps[i];
}

// MODIFIERS

void mesh::clear() {
	if (ps == nullptr) {
		return;
	}

	for (size_t i = 0; i < N; ++i) {
		delete ps[i];
	}
	free(ps);
	ps = nullptr;
}

// SETTERS

void mesh::set_elasticity(float ke) {
	Ke = ke;
}

void mesh::set_damping(float kd) {
	Kd = kd;
}

// GETTERS

float mesh::get_elasticity() const {
	return Ke;
}

float mesh::get_damping() const {
	return Kd;
}

size_t mesh::size() const {
	return N;
}

const mesh_type& mesh::get_type() const {
	return mt;
}

mesh_particle **mesh::get_particles() {
	return ps;
}

particles::mesh_particle *const *mesh::get_particles() const {
	return ps;
}

} // -- namespace meshes
} // -- namespace physim
