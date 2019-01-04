#include <physim/fluids/newtonian.hpp>

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

#define sq(x) ((x)*(x))
#define inv(x) (1.0f/(x))

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
		ps[i].density += ps[j].mass*kernel_density(d2s[j_it]);	\
	}															\
	ps[i].density += ps[i].mass*kernel_density(0.0f);			\
	ps[i].pressure = sq(speed_sound)*(ps[i].density - density);

#define update_force(i, neighs, d2s)								\
	/* total acceleration */										\
	vec3 acc;														\
	/* auxiliary vectors */											\
	vec3 part_i_to_j, pressure_dir, press_acc, visc_acc;			\
	for (size_t j_it = 0; j_it < neighs.size(); ++j_it) {			\
		size_t j = neighs[j_it];									\
		/* pressure acceleration */									\
		__pm3_sub_v_v(part_i_to_j, ps[j].cur_pos, ps[i].cur_pos);	\
		float Pij = -ps[j].mass*(									\
			ps[i].pressure*inv(sq(ps[i].density)) +					\
			ps[j].pressure*inv(sq(ps[j].density))					\
		);															\
		kernel_pressure(part_i_to_j, d2s[j_it], pressure_dir);		\
		__pm3_assign_vs(press_acc, pressure_dir,Pij);				\
		__pm3_add_acc_v(acc, press_acc);							\
		/* viscosity acceleration */								\
		float Vij = ps[i].density*ps[j].density;					\
		Vij *= viscosity*ps[j].mass*inv(Vij);						\
		Vij *= kernel_viscosity(d2s[j_it]);							\
		__pm3_sub_v_v_mul_s											\
		(visc_acc, ps[i].cur_vel,ps[j].cur_vel, Vij);				\
		/* accumulate acceleration */								\
		__pm3_add_acc_v(acc, visc_acc);								\
	}																\
	__pm3_assign_vs(ps[i].force, acc, ps[i].mass);

// PUBLIC

newtonian::newtonian() : fluid() {
}

newtonian::~newtonian() {
}

// MODIFIERS

void newtonian::update_forces() {
	make_partition();

	// Compute neighbour lists, and squared distances
	// between a particle and its neighbours.
	// We need a dynamic list for the neighbours, but a
	// fixed-size list of these lists will suffice.
	vector<vector<size_t> > all_neighs(N);
	vector<vector<float> > all_d2s(N);

	// Obtain neighbours list. Then filter them to have only
	// the closest neighbours.
	for (size_t i = 0; i < N; ++i) {
		make_neighbours_lists(i, all_neighs[i], all_d2s[i]);
	}

	// compute density and pressure of each particle
	for (size_t i = 0; i < N; ++i) {
		initialise_densities_pressures(i, all_neighs[i], all_d2s[i]);
	}

	// compute forces of the fluid (due to pressure and viscosity)
	for (size_t i = 0; i < N; ++i) {
		update_force(i, all_neighs[i], all_d2s[i]);
	}
}

void newtonian::update_forces(size_t n) {
	if (n == 1) {
		update_forces();
		return;
	}

	// Compute neighbour lists, and squared distances
	// between a particle and its neighbours.
	// We need a dynamic list for the neighbours, but a
	// fixed-size list of these lists will suffice.
	vector<vector<size_t> > all_neighs(N);
	vector<vector<float> > all_d2s(N);

	// Obtain neighbours list. Then filter them to have only
	// the closest neighbours.
	#pragma omp parallel for num_threads(n)
	for (size_t i = 0; i < N; ++i) {
		make_neighbours_lists(i, all_neighs[i], all_d2s[i]);
	}

	// compute density and pressure of each particle
	#pragma omp parallel for num_threads(n)
	for (size_t i = 0; i < N; ++i) {
		initialise_densities_pressures(i, all_neighs[i], all_d2s[i]);
	}

	// compute forces of the fluid (due to pressure and viscosity)
	#pragma omp parallel for num_threads(n)
	for (size_t i = 0; i < N; ++i) {
		update_force(i, all_neighs[i], all_d2s[i]);
	}
}

} // -- namespace fluids
} // -- namespace physim

