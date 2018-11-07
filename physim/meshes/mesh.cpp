#include <physim/meshes/mesh.hpp>

// C includes
#include <assert.h>
#include <stdlib.h>

namespace physim {
using namespace particles;

namespace meshes {

// PUBLIC

mesh::mesh() {
	friction = 0.2f;
	bouncing = 0.8f;
	Ke = 100.0f;
	Kd = 0.05f;
	N = 0;
	ps = nullptr;
}
mesh::mesh(float ke, float kd) {
	friction = 0.2f;
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

void mesh::allocate(size_t n, float Kg) {
	bool delete_particles;
	if (ps != nullptr) {
		// clear and allocate only if necessary
		if (n == N) {
			delete_particles = true;
		}
		else {
			clear();
			N = n;
			ps = (mesh_particle **)malloc(N*sizeof(mesh_particle *));
			delete_particles = false;
		}
	}
	else {
		N = n;
		ps = (mesh_particle **)malloc(N*sizeof(mesh_particle *));
		delete_particles = false;
	}

	for (size_t i = 0; i < N; ++i) {
		if (delete_particles) {
			delete ps[i];
		}
		ps[i] = new mesh_particle();
		ps[i]->index = i;
		ps[i]->mass = Kg/N;
	}
}

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
		ps[i]->mass = Kg/N;
	}
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
