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

#define sq(x) (x*x)

namespace physim {
using namespace math;
using namespace particles;
using namespace structures;

namespace fluids {

// PROTECTED

#define make_neighbours_lists(i, neighs, d2s)					\
	tree->get_indices(ps[i].cur_pos, R, neighs);				\
	d2s = vector<float>(neighs.size());							\
	/* resort by distances: put those farther than R
	 * at the back of the vector */								\
	size_t lim = neighs.size();									\
	size_t j_it = 0;											\
	while (j_it < lim) {										\
		size_t j = neighs[j_it];								\
		d2s[j_it] = __pm3_dist2(ps[i].cur_pos, ps[j].cur_pos);	\
		if (d2s[j_it] > sq(R) or j == i) {						\
			/* Put this element at the end. (to be removed)
			 * Do not advance 'j_it' */							\
			swap(neighs[j_it], neighs[lim - 1]);				\
			swap(d2s[j_it], d2s[lim - 1]);						\
			--lim;												\
		}														\
		else {													\
			/* this element is OK. Move forward. */				\
			++j_it;												\
		}														\
	}															\
	/* remove last elements */									\
	neighs.erase( neighs.begin() + lim, neighs.end() );			\
	d2s.erase( d2s.begin() + lim, d2s.end() );

#define initialise_densities_pressures(i, neighs, d2s)			\
	ps[i].density = 0.0f;										\
	/* iterate over the list of neighbours.
	 * initialise density, pressure, and store squared
	 * distance */												\
	for (size_t j_it = 0; j_it < neighs.size(); ++j_it) {		\
		size_t j = neighs[j_it];								\
		ps[i].density += ps[j].mass*kernel(d2s[j_it]);			\
	}															\
	ps[i].density += ps[i].mass*kernel(0.0f);					\
	ps[i].pressure = sq(speed_sound)*(ps[i].density - density);

#define update_force(i, neighs, d2s)							\
	vec3 dir, aux;												\
	vec3 acc; /* total acceleration */							\
	for (size_t j_it = 0; j_it < neighs.size(); ++j_it) {		\
		size_t j = neighs[j_it];								\
		__pm3_sub_v_v(dir, ps[j].cur_pos, ps[i].cur_pos);		\
		float Pij = -ps[j].mass*(								\
			ps[i].pressure*(1.0f/sq(ps[i].density)) +			\
			ps[j].pressure*(1.0f/sq(ps[j].density))				\
		);														\
		kernel_pressure(dir, d2s[j_it], aux);					\
		__pm3_add_acc_vs(acc, aux, Pij);						\
		float visc_coef = ps[i].density*ps[j].density;			\
		visc_coef *= viscosity*ps[j].mass*(1.0f/visc_coef);		\
		visc_coef *= kernel_viscosity(d2s[j_it]);				\
		__pm3_sub_v_v_mul_s										\
		(aux, ps[i].cur_vel, ps[j].cur_vel,visc_coef);			\
		__pm3_add_acc_v(acc, aux);								\
	}															\
	__pm3_assign_vs(ps[i].force, acc, ps[i].mass);

// PUBLIC

fluid::fluid() {
	N = 0;
	ps = nullptr;
	tree = nullptr;

	kernel_scalar_function emptyS =
	[](float) { return 1.0f; };
	kernel_vectorial_function emptyV =
	[](const vec3&, float, vec3&) -> void { };

	// use empty kernels, to avoid segmentation
	// faults and things like these
	kernel = emptyS;
	kernel_pressure = emptyV;
	kernel_viscosity = emptyS;
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
(size_t n, float vol, float dens, float visc, float r, float cs)
{
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
		cerr << "fluid::allocate (" << __LINE__ << ") - Error:" << endl;
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

void fluid::update_forces() {
	make_partition();

	// Compute neighbour lists, and squared distances
	// between a particle and its neighbours.
	// We need a dynamic list for the neighbours, but a
	// fixed-size list of these lists will suffice.
	vector<vector<size_t> > neighs(N);
	vector<vector<float> > d2s(N);

	cout << "----------------" << endl;

	// Obtain neighbours list. Then filter them to have only
	// the closest neighbours.
	for (size_t i = 0; i < N; ++i) {
		make_neighbours_lists(i, neighs[i], d2s[i]);
	}

	// compute density and pressure of each particle
	for (size_t i = 0; i < N; ++i) {
		initialise_densities_pressures(i, neighs[i], d2s[i]);
	}

	// compute forces of the fluid (due to pressure and viscosity)
	for (size_t i = 0; i < N; ++i) {
		update_force(i, neighs[i], d2s[i]);
	}
}

void fluid::update_forces(size_t n) {
	if (n == 1) {
		update_forces();
		return;
	}

	// Compute neighbour lists, and squared distances
	// between a particle and its neighbours.
	// We need a dynamic list for the neighbours, but a
	// fixed-size list of these lists will suffice.
	vector<vector<size_t> > neighs(N);
	vector<vector<float> > d2s(N);

	// Obtain neighbours list. Then filter them to have only
	// the closest neighbours.
	#pragma omp parallel for num_threads(n)
	for (size_t i = 0; i < N; ++i) {
		make_neighbours_lists(i, neighs[i], d2s[i]);
	}

	// compute density and pressure of each particle
	#pragma omp parallel for num_threads(n)
	for (size_t i = 0; i < N; ++i) {
		initialise_densities_pressures(i, neighs[i], d2s[i]);
	}

	// compute forces of the fluid (due to pressure and viscosity)
	#pragma omp parallel for num_threads(n)
	for (size_t i = 0; i < N; ++i) {
		update_force(i, neighs[i], d2s[i]);
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

void fluid::set_kernel(
	const kernel_scalar_function& W,
	const kernel_vectorial_function& W_p,
	const kernel_scalar_function& W_v
)
{
	kernel = W;
	kernel_pressure = W_p;
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

