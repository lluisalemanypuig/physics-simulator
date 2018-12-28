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

#define Pij(i,j) -ps[j].mass*(					\
	ps[i].pressure*(1.0f/sq(ps[i].density)) +	\
	ps[j].pressure*(1.0f/sq(ps[j].density))		\
	)

#define Vij(i,j) viscosity*ps[j].mass*(			\
	(ps[i].cur_vel - ps[j].cur_vel)*			\
	(1.0f/(ps[i].density*ps[j].density)))

namespace physim {
using namespace math;
using namespace particles;
using namespace structures;

namespace fluids {

// PUBLIC

fluid::fluid() {
	N = 0;
	ps = nullptr;
	tree = nullptr;

	kernel_function empty =
	[](const fluid_particle&, const fluid_particle&, float) -> float
	{ return 1.0f; };

	// use empty kernels, to avoid segmentation
	// faults and things like these
	kernel = empty;
	kernel_pressure = empty;
	kernel_viscosity = empty;
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
		// Oh, Lord... I didn't know that I had to do this...
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

	const float R2 = R*R;

	// Compute neighbour lists, and squared distances
	// between a particle and its neighbours.
	// We need a dynamic list for the neighbours, but a
	// fixed-size list of these lists will suffice.
	vector<vector<size_t> > neighs(N);
	vector<vector<float> > d2s(N);

	for (size_t i = 0; i < N; ++i) {
		tree->get_indices(ps[i].cur_pos, R, neighs[i]);
		d2s[i] = vector<float>(neighs[i].size());

		// resort by distances: put those farther than R
		// at the back of the vector
		size_t lim = neighs[i].size();
		size_t j_it = 0;
		while (j_it < lim) {

			size_t j = neighs[i][j_it];
			d2s[i][j_it] = __pm3_dist2(ps[i].cur_pos, ps[j].cur_pos);

			if (d2s[i][j_it] > R2) {
				// Put this element at the end.
				// Do not advance 'j_it'
				std::swap(neighs[i][j_it], neighs[i][lim - 1]);
				std::swap(d2s[i][j_it], d2s[i][lim - 1]);
				--lim;
			}
			else {
				// this element is OK. Move forward.
				++j_it;
			}
		}

		// remove last elements
		neighs[i].erase( neighs[i].begin() + lim, neighs[i].end() );
		d2s[i].erase( d2s[i].begin() + lim, d2s[i].end() );
	}

	// compute density and pressure of each particle,
	// store squared distance
	for (size_t i = 0; i < N; ++i) {
		ps[i].density = 0.0f;

		// iterate over the list of neighbours.
		// initialise density, pressure, and store squared distance
		for (size_t j_it = 0; j_it < neighs[i].size(); ++j_it) {
			size_t j = neighs[i][j_it];
			ps[i].density += ps[j].mass*kernel(ps[i], ps[j], d2s[i][j_it]);
		}

		ps[i].pressure = sq(speed_sound)*(ps[i].density - density);
	}

	// compute forces of the fluid (due to pressure and viscosity)
	for (size_t i = 0; i < N; ++i) {

		float aP = 0.0f;	// acceleration from pressure
		vec3 aV;	// acceleration from viscosity

		for (size_t j_it = 0; j_it < neighs[i].size(); ++j_it) {
			size_t j = neighs[i][j_it];

			float Pij = -ps[j].mass*(
				ps[i].pressure*(1.0f/sq(ps[i].density)) +
				ps[j].pressure*(1.0f/sq(ps[j].density))
			);
			aP += Pij(i,j)*kernel_pressure(ps[i], ps[j], d2s[i][j_it]);

			float visc_coef = ps[i].density*ps[j].density;
			visc_coef *= viscosity*ps[j].mass*(1.0f/visc_coef);
			visc_coef *= kernel_viscosity(ps[i], ps[j], d2s[i][j_it]);
			__pm3_sub_v_v_mul_s(aV, ps[i].cur_vel, ps[j].cur_vel, visc_coef);
		}

		__pm3_add_acc_v(ps[i].force, aV);
	}
}

void fluid::update_forces(size_t n) {
	if (n == 1) {
		update_forces();
		return;
	}

	make_partition();

	const float R2 = R*R;

	// Compute neighbour lists, and squared distances
	// between a particle and its neighbours.
	// We need a dynamic list for the neighbours, but a
	// fixed-size list of these lists will suffice.
	vector<vector<size_t> > neighs(N);
	vector<vector<float> > d2s(N);

	#pragma omp parallel for num_threads(n)
	for (size_t i = 0; i < N; ++i) {
		tree->get_indices(ps[i].cur_pos, R, neighs[i]);
		d2s[i] = vector<float>(neighs[i].size());

		// resort by distances: put those farther than R
		// at the back of the vector
		size_t lim = neighs[i].size();
		size_t j_it = 0;
		while (j_it < lim) {

			size_t j = neighs[i][j_it];
			d2s[i][j_it] = __pm3_dist2(ps[i].cur_pos, ps[j].cur_pos);

			if (d2s[i][j_it] > R2 or ps[j].index == ps[i].index) {
				// Put this element at the end.
				// Do not advance 'j_it'
				std::swap(neighs[i][j_it], neighs[i][lim - 1]);
				std::swap(d2s[i][j_it], d2s[i][lim - 1]);
				--lim;
			}
			else {
				// this element is OK. Move forward.
				++j_it;
			}
		}

		// remove last elements
		neighs[i].erase( neighs[i].begin() + lim, neighs[i].end() );
		d2s[i].erase( d2s[i].begin() + lim, d2s[i].end() );
	}

	// compute density and pressure of each particle
	#pragma omp parallel for num_threads(n)
	for (size_t i = 0; i < N; ++i) {
		ps[i].density = 0.0f;

		// iterate over the list of neighbours.
		// initialise density, pressure, and store squared distance
		for (size_t j_it = 0; j_it < neighs[i].size(); ++j_it) {
			size_t j = neighs[i][j_it];
			ps[i].density += ps[j].mass*kernel(ps[i], ps[j], d2s[i][j_it]);
		}

		ps[i].pressure = sq(speed_sound)*(ps[i].density - density);
	}

	// compute forces of the fluid (due to pressure and viscosity)
	#pragma omp parallel for num_threads(n)
	for (size_t i = 0; i < N; ++i) {

		float aP = 0.0f;	// acceleration from pressure
		float aV = 0.0f;	// acceleration from viscosity

		for (size_t j_it = 0; j_it < neighs[i].size(); ++j_it) {
			size_t j = neighs[i][j_it];
			aP += Pij(i,j)*kernel_pressure(ps[i], ps[j], d2s[i][j_it]);
			aV += Pij(i,j)*kernel_viscosity(ps[i], ps[j], d2s[i][j_it]);
		}

		ps[i].force += (aP + aV)*ps[i].mass;
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

void fluid::set_kernel
(const kernel_function& W, const kernel_function& W_p, const kernel_function& W_v)
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

