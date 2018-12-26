#include <physim/fluids/fluid.hpp>

// C includes
#include <stdlib.h>

// C++ includes
#include <vector>
using namespace std;

#define speed_sound 343.0f
#define speed_sound2 speed_sound*speed_sound

namespace physim {
using namespace particles;
using namespace structures;

namespace fluids {

// PUBLIC

fluid::fluid() {
	N = 0;
	volume = 0.0f;
	density = 0.0f;
	viscosity = 0.0f;
	ps = nullptr;
}

fluid::~fluid() {
	clear();
}

// OPERATORS

particles::fluid_particle *fluid::operator[] (size_t i) {
	return ps[i];
}

const particles::fluid_particle *fluid::operator[] (size_t i) const {
	return ps[i];
}

// MODIFIERS

void fluid::allocate(size_t n, float vol, float dens, float visc, float r) {
	if (tree == nullptr) {
		tree = new octree();
	}
	else {
		tree->clear();
	}

	R = r;
	volume = vol;
	density = dens;
	viscosity = visc;

	float mass_per_particle = (density*volume)/n;

	// particles in the array should be
	// initialised only, or reallocated?
	bool new_particles;

	if (ps != nullptr) {
		// clear and allocate only if necessary
		if (n == N) {
			// do not deallocate, because we want
			new_particles = false;
		}
		else {
			clear();
			N = n;
			ps = static_cast<fluid_particle **>(malloc(N*sizeof(fluid_particle *)));
			new_particles = true;
		}
	}
	else {
		N = n;
		ps = static_cast<fluid_particle **>(malloc(N*sizeof(fluid_particle *)));
		new_particles = true;
	}

	for (size_t i = 0; i < N; ++i) {
		if (new_particles) {
			ps[i] = new fluid_particle();
		}
		else {
			ps[i]->init();
		}
		ps[i]->index = i;
		ps[i]->mass = mass_per_particle;
	}
}

void fluid::clear() {
	if (ps != nullptr) {
		free(ps);
		ps = nullptr;
	}
}

void fluid::update_forces() {

}

void fluid::make_partition() {

}

// SETTERS

void fluid::set_kernel
(const kernel_function& W, const kernel_function& nW, const kernel_function& n2W)
{
	kernel = W;
	kernel_gr = nW;
	kernel_gr2 = n2W;
}

// GETTERS

size_t fluid::size() const {
	return N;
}

float fluid::get_volume() const {
	return volume;
}

float fluid::get_density() const {
	return density;
}

float fluid::get_viscosity() const {
	return viscosity;
}

fluid_particle **fluid::get_particles() {
	return ps;
}
fluid_particle *const *fluid::get_particles() const {
	return ps;
}

} // -- namespace fluids
} // -- namespace physim

