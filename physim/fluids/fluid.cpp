#include <physim/fluids/fluid.hpp>

// C includes
#include <stdlib.h>

namespace physim {
using namespace particles;

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

void fluid::init(size_t n, float vol, float dens, float visc) {
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
			ps = (fluid_particle **)malloc(N*sizeof(fluid_particle *));
			new_particles = true;
		}
	}
	else {
		N = n;
		ps = (fluid_particle **)malloc(N*sizeof(fluid_particle *));
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

// SETTERS

void fluid::set_pressure_kernel(const kernel_pair& kp) {
	kernel_pressure = kp;
}

void fluid::set_viscosity_kernel(const kernel_pair& kp) {
	kernel_viscosity = kp;
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

} // -- namespace fluids
} // -- namespace physim

