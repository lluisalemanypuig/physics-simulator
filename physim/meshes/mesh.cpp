#include <physim/meshes/mesh.hpp>

// C includes
#include <assert.h>
#include <stdlib.h>

// C++ includes
#include <iostream>
using namespace std;

namespace physim {
using namespace particles;

namespace meshes {

// PUBLIC

mesh::mesh() {
	N = 0;
	ps = nullptr;
	friction = 0.2f;
	bouncing = 0.8f;
	Ke = 100.0f;
	Kd = 0.05f;
}
mesh::mesh(float ke, float kd) {
	N = 0;
	ps = nullptr;
	friction = 0.2f;
	bouncing = 0.8f;
	Ke = ke;
	Kd = kd;
}

mesh::~mesh() {
	clear();
}

// OPERATORS

mesh_particle& mesh::operator[] (size_t i) {
	assert(i < N);
	return ps[i];
}

const mesh_particle& mesh::operator[] (size_t i) const {
	assert(i < N);
	return ps[i];
}

// MODIFIERS

void mesh::allocate(size_t n, float Kg) {

	if (ps != nullptr and n != N) {
		// clear and allocate only if necessary
		clear();
		N = n;
		ps = static_cast<mesh_particle *>(malloc(N*sizeof(mesh_particle)));
	}
	else {
		N = n;
		ps = static_cast<mesh_particle *>(malloc(N*sizeof(mesh_particle)));
	}

	if (ps == nullptr) {
		cerr << "mesh::allocate (" << __LINE__ << ") - Error:" << endl;
		cerr << "    Could not allocate memory for "
			 << N << " particles (" << N*sizeof(mesh_particle) << " bytes)"
			 << endl;
		return;
	}

	for (size_t i = 0; i < N; ++i) {
		// ATENTION! This needs the <iostream> header included
		new (&(ps[i])) mesh_particle();

		ps[i].index = i;
		ps[i].mass = Kg/N;
	}
}

void mesh::clear() {
	if (ps != nullptr) {
		free(ps);
		ps = nullptr;
	}
}

// SETTERS

void mesh::set_elasticity(float ke) {
	Ke = ke;
}
void mesh::set_damping(float kd) {
	Kd = kd;
}

void mesh::set_friction(float f) {
	friction = f;
}
void mesh::set_bouncing(float b) {
	bouncing = b;
}

// GETTERS

float mesh::get_elasticity() const {
	return Ke;
}
float mesh::get_damping() const {
	return Kd;
}

float mesh::get_friction() const {
	return friction;
}
float mesh::get_bouncing() const {
	return bouncing;
}

void mesh::set_mass(float Kg) {
	assert(ps != nullptr);

	for (size_t i = 0; i < N; ++i) {
		ps[i].mass = Kg/N;
	}
}

size_t mesh::size() const {
	return N;
}

const mesh_type& mesh::get_type() const {
	return mt;
}

mesh_particle *mesh::get_particles() {
	return ps;
}
const mesh_particle *mesh::get_particles() const {
	return ps;
}

} // -- namespace meshes
} // -- namespace physim
