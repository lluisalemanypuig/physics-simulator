#include <physim/fluids/fluid.hpp>

// C includes
#include <stdlib.h>
#include <omp.h>

// C++ includes
#include <iostream>
#include <vector>
using namespace std;

// physim includes
#include <physim/math/private/math3.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
using namespace math;
using namespace particles;
using namespace structures;

namespace fluids {

// PROTECTED

// PUBLIC

fluid::fluid() {
	N = 0;
	ps = nullptr;
	tree = nullptr;

	// use empty kernels, to avoid segmentation
	// faults and things like these
	kernel_density = [](float) { return 1.0f; };
	kernel_pressure = [](const vec3&, float, vec3&) -> void { };
	kernel_viscosity = [](float) { return 1.0f; };
}

fluid::~fluid() {
	clear();

	if (tree != nullptr) {
		delete tree;
		tree = nullptr;
	}
}

// OPERATORS

particles::fluid_particle *fluid::operator[] (size_t i) {
	return &ps[i];
}

const particles::fluid_particle *fluid::operator[] (size_t i) const {
	return &ps[i];
}

// MODIFIERS

void fluid::allocate
(size_t n, float vol, float dens, float visc, float r, float cs) {
	if (tree == nullptr) {
		tree = new octree();
	}

	speed_sound = cs;
	R = r;
	volume = vol;
	density = dens;
	viscosity = visc;

	float mass_per_particle = (density*volume)/n;

	if (ps != nullptr and n != N) {
		// clear and allocate only if necessary
		clear();
		N = n;
		ps = static_cast<fluid_particle *>(malloc(N*sizeof(fluid_particle)));
	}
	else {
		N = n;
		ps = static_cast<fluid_particle *>(malloc(N*sizeof(fluid_particle)));
	}

	if (ps == nullptr) {
		cerr << "fluid::allocate (:" << __LINE__ << ") - Error:" << endl;
		cerr << "    Could not allocate memory for "
			 << N << " particles (" << N*sizeof(fluid_particle) << " bytes)"
			 << endl;
		return;
	}

	for (size_t i = 0; i < N; ++i) {
		// ATTENTION! This needs the <iostream> header included
		new (&(ps[i])) fluid_particle();

		ps[i].index = i;
		ps[i].mass = mass_per_particle;
	}
}

void fluid::clear() {
	if (ps != nullptr) {
		free(ps);
		ps = nullptr;
	}
}

void fluid::make_partition() {
	tree->clear();

	// compute offset of position w.r.t. to the
	// beginning of its struct
	size_t base = reinterpret_cast<size_t>( &ps[0] );
	size_t member_pos = reinterpret_cast<size_t>( &ps[0].cur_pos );
	size_t offset = member_pos - base;

	tree->init(&ps[0], N, 1, sizeof(fluid_particle), offset);
}

// SETTERS

void fluid::set_kernel_density(const kernel_scalar_function& W_d) {
	kernel_density = W_d;
}

void fluid::set_kernel_pressure(const kernel_vectorial_function& W_p) {
	kernel_pressure = W_p;
}

void fluid::set_kernel_viscosity(const kernel_scalar_function& W_v) {
	kernel_viscosity = W_v;
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

fluid_particle *fluid::get_particles() {
	return ps;
}
const fluid_particle *fluid::get_particles() const {
	return ps;
}

} // -- namespace fluids
} // -- namespace physim

