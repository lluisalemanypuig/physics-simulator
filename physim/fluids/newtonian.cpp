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

#define EXPERIMENTAL

namespace physim {
using namespace math;
using namespace particles;
using namespace structures;

namespace fluids {

// PROTECTED

#define sq(x) ((x)*(x))
#define inv(x) (1.0f/(x))

void newtonian::make_neighbours_lists
(size_t i, vector<size_t>& neihgs, vector<float>& d2s)
const
{
	//cout << "    " << i << ":" << endl;
	for (size_t j = 0; j < N; ++j) {
		if (i == j) {
			continue;
		}
		float d2 = __pm3_dist2(ps[i].cur_pos, ps[j].cur_pos);
		if (d2 <= sq(R)) {
			neihgs.push_back(j);
			d2s.push_back(d2);
		}
	}

	/*cout << "        neighbourhood size: " << all_neighs[i].size() << endl;
	cout << "           ";
	for (size_t j = 0; j < all_neighs[i].size(); ++j) {
		cout << " " << all_neighs[i][j];
	}
	cout << endl;*/
}

void newtonian::make_neighbours_lists_tree
(size_t i, vector<size_t>& neighs, vector<float>& d2s) const
{
	tree->get_indices(ps[i].cur_pos, R, neighs);
	d2s = vector<float>(neighs.size());
	/* resort by distances: put those farther than R
	 * at the back of the vector */
	size_t lim = neighs.size();
	size_t j_it = 0;
	while (j_it < lim) {
		size_t j = neighs[j_it];
		d2s[j_it] = __pm3_dist2(ps[i].cur_pos, ps[j].cur_pos);
		if (d2s[j_it] > sq(R) or j == i) {
			/* Put this element at the end. (to be removed)
			 * Do not advance 'j_it' */
			swap(neighs[j_it], neighs[lim - 1]);
			swap(d2s[j_it], d2s[lim - 1]);
			--lim;
		}
		else {
			/* this element is OK. Move forward. */
			++j_it;
		}
	}
	/* remove last elements */
	neighs.erase( neighs.begin() + lim, neighs.end() );
	d2s.erase( d2s.begin() + lim, d2s.end() );
}

void newtonian::initialise_density_pressure
(size_t i, const vector<size_t>& neighs, const vector<float>& d2s)
{
	//cout << "    " << i << ":" << endl;

	ps[i].density = 0.0f;
	/* iterate over the list of neighbours.
	 * initialise density, pressure, and store squared
	 * distance */
	for (size_t j_it = 0; j_it < neighs.size(); ++j_it) {
		size_t j = neighs[j_it];
		float d2 = d2s[j_it];

		ps[i].density += ps[j].mass*kernel_density(d2);
		/*cout << "        ++ " << ps[i].density
			 << ". ! " << j << ": " << ps[j].mass*kernel_density(d2)
			 << endl;
		cout << "            ps[" << j << "].mass= " << ps[j].mass << endl;
		cout << "            ps[" << j << "].cur_pos= "
			 << __pm3_out(ps[j].cur_pos) << endl;
		cout << "            all_d2s[" << i << "][" << j_it << "]= "
			 << d2 << endl;
		cout << "            W_density()= "
			 << kernel_density(d2) << endl;*/

	}
	ps[i].density += ps[i].mass*kernel_density(0.0f);
	/*cout << "        D: ++ " << ps[i].density
		 << ". ! " << i << ": " << ps[i].mass*kernel_density(0.0f)
		 << endl;*/

	ps[i].pressure = sq(speed_sound)*(ps[i].density - density);
	//cout << "        P: " << ps[i].pressure << endl;
}

void newtonian::update_force
(size_t i, const vector<size_t>& neighs, const vector<float>& d2s)
{
	/* total acceleration */
	vec3 acc;
	/* auxiliary vectors */
	vec3 part_i_to_j, pressure_dir, press_acc, visc_acc;
	for (size_t j_it = 0; j_it < neighs.size(); ++j_it) {
		size_t j = neighs[j_it];

		/* pressure acceleration */
		__pm3_sub_v_v(part_i_to_j, ps[j].cur_pos, ps[i].cur_pos);
		float Pij = -ps[j].mass*(
			ps[i].pressure*inv(sq(ps[i].density)) +
			ps[j].pressure*inv(sq(ps[j].density))
		);
		kernel_pressure(part_i_to_j, d2s[j_it], pressure_dir);
		__pm3_assign_vs(press_acc, pressure_dir,Pij);
		__pm3_add_acc_v(acc, press_acc);

		/* viscosity acceleration */
		float Vij = ps[i].density*ps[j].density;
		Vij *= ps[j].mass*inv(Vij);
		Vij *= kernel_viscosity(d2s[j_it]);
		__pm3_sub_v_v_mul_s (visc_acc, ps[i].cur_vel,ps[j].cur_vel, Vij);
		__pm3_add_acc_v(acc, visc_acc);
	}

	/* take common factor to viscosity */
	__pm3_mul_acc_s(visc_acc, viscosity);

	/* compute force in particle*/
	__pm3_assign_vs(ps[i].force, acc, ps[i].mass);
}

// PUBLIC

newtonian::newtonian() : fluid() {
}

newtonian::~newtonian() {
}

// MODIFIERS

void newtonian::update_forces() {
	// Compute neighbour lists, and squared distances
	// between a particle and its neighbours.
	// We need a dynamic list for the neighbours, but a
	// fixed-size list of these lists will suffice.
	vector<vector<size_t> > all_neighs(N);
	vector<vector<float> > all_d2s(N);

#if defined (SAFE)
	for (size_t i = 0; i < N; ++i) {
		make_neighbours_lists(i, all_neighs[i], all_d2s[i]);
	}
#endif

#if defined (EXPERIMENTAL)
	make_partition();
	for (size_t i = 0; i < N; ++i) {
		make_neighbours_lists_tree(i, all_neighs[i], all_d2s[i]);
	}
#endif

	//cout << "Density and pressure" << endl;

	// compute density and pressure of each particle
	for (size_t i = 0; i < N; ++i) {
		initialise_density_pressure(i, all_neighs[i], all_d2s[i]);
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

#if defined (SAFE)
	#pragma omp parallel for num_threads(n)
	for (size_t i = 0; i < N; ++i) {
		make_neighbours_lists(i, all_neighs[i], all_d2s[i]);
	}
#endif

#if defined (EXPERIMENTAL)
	make_partition();
	#pragma omp parallel for num_threads(n)
	for (size_t i = 0; i < N; ++i) {
		make_neighbours_lists_tree(i, all_neighs[i], all_d2s[i]);
	}
#endif

	//cout << "Density and pressure" << endl;

	// compute density and pressure of each particle
	#pragma omp parallel for num_threads(n)
	for (size_t i = 0; i < N; ++i) {
		initialise_density_pressure(i, all_neighs[i], all_d2s[i]);
	}

	// compute forces of the fluid (due to pressure and viscosity)
	#pragma omp parallel for num_threads(n)
	for (size_t i = 0; i < N; ++i) {
		update_force(i, all_neighs[i], all_d2s[i]);
	}
}

} // -- namespace fluids
} // -- namespace physim

