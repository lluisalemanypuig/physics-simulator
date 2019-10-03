/*********************************************************************
 * Real-time physics simulation project
 * Copyright (C) 2018-2019 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

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
#include <physim/math/private/numeric.hpp>
#include <physim/math/vec3.hpp>

// neighbour computation
#define SAFE_NEIGH			0
#define EXPL_NEIGH			1

// output intermediate results
#define OUTPUT_NEIGHBOURS	0
#define OUTPUT_DENS			0
#define OUTPUT_PRESS		0
#define OUTPUT_ACCEL_PRESS	0
#define OUTPUT_ACCEL_VISC	0
#define OUTPUT_FORCE		0
#define OUTPUT_ACCEL_FORCE (OUTPUT_ACCEL_PRESS + OUTPUT_ACCEL_VISC + OUTPUT_FORCE)

#define OUTPUT_PRESS_DENS (OUTPUT_DENS + OUTPUT_PRESS)
#define OUTPUT (OUTPUT_NEIGHBOURS + OUTPUT_DENS + OUTPUT_PRESS + OUTPUT_ACCEL_PRESS + OUTPUT_ACCEL_VISC + OUTPUT_FORCE)

#if (SAFE_NEIGH == 1) && (EXPL_NEIGH == 1)
#error("Can't use both safe and experimental neighbour retrieval")
#endif

namespace physim {
using namespace math;
using namespace particles;
using namespace structures;

namespace fluids {

// PROTECTED

void newtonian::make_neighbours_lists
(size_t i, vector<size_t>& neighs, vector<float>& d2s)
const
{
	//cout << "    " << i << ":" << endl;
	for (size_t j = 0; j < N; ++j) {
		if (i == j) {
			continue;
		}
		float d2 = __pm3_dist2(ps[i].cur_pos, ps[j].cur_pos);
		if (d2 <= __pm_sq(R)) {
			neighs.push_back(j);
			d2s.push_back(d2);
		}
	}
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
		if (d2s[j_it] > __pm_sq(R) or j == i) {
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

#if OUTPUT_NEIGHBOURS == 1
	cout << "Neighbours:" << endl;
	cout << "    particle " << i << " has " << neighs.size() << " neighbours" << endl;
	cout << "    particle is at: " << __pm3_out(ps[i].cur_pos) << endl;
	cout << "    neighbours:" << endl;
	for (size_t j_it = 0; j_it < neighs.size(); ++j_it) {
		size_t j = neighs[j_it];
		cout << "        " << j << ": " << __pm3_out(ps[j].cur_pos)
			 << " .. d2= " << d2s[j_it] << endl;
	}
#endif
}

void newtonian::initialise_density_pressure
(size_t i, const vector<size_t>& neighs, const vector<float>& d2s)
{
#if OUTPUT_PRESS_DENS == 1
	cout << "    ** particle " << i << endl;
#endif

	ps[i].density = 0.0f;
	/* iterate over the list of neighbours.
	 * initialise density, pressure */
	for (size_t j_it = 0; j_it < neighs.size(); ++j_it) {
		size_t j = neighs[j_it];
		float d2 = d2s[j_it];

#if OUTPUT_DENS == 1
		cout << "        for neighbour " << j << " at distance^2= " << d2 << endl;
		cout << "            density before= " << ps[i].density << endl;
#endif

		ps[i].density += ps[j].mass*kernel_density(d2);

#if OUTPUT_DENS == 1
		cout << "            density after= " << ps[i].density << endl;
		cout << "            contribution=   " << ps[j].mass*kernel_density(d2) << endl;
		cout << "                ps[" << j << "].mass= " << ps[j].mass << endl;
		cout << "                ps[" << j << "].cur_pos= "
			 << __pm3_out(ps[j].cur_pos) << endl;
		cout << "                d2s[" << j_it << "]= "
			 << d2 << endl;
		cout << "                W_density()= "
			 << kernel_density(d2) << endl;
#endif

	}
	ps[i].density += ps[i].mass*kernel_density(0.0f);

#if OUTPUT_DENS == 1
	cout << "    final density= " << ps[i].density << endl;
	cout << "        self-contribution= "
		 << ps[i].mass*kernel_density(0.0f) << endl;
#endif

	ps[i].pressure = __pm_sq(speed_sound)*(ps[i].density - density);

#if OUTPUT_PRESS == 1
	cout << "    pressure= " << ps[i].pressure << endl;
	cout << "        diff= " << ps[i].density - density << endl;
#endif
}

void newtonian::update_force
(size_t i, const vector<size_t>& neighs, const vector<float>& d2s)
{
#if OUTPUT_ACCEL_FORCE > 0
	cout << "    ** particle " << i << ". # neighbours= " << neighs.size() << endl;
#endif

	/* total acceleration */
	vec3 acc;
	/* auxiliary vectors */
	vec3 part_i_to_j, pressure_dir, press_acc, visc_acc;


	for (size_t j_it = 0; j_it < neighs.size(); ++j_it) {
		size_t j = neighs[j_it];
		float d2 = d2s[j_it];

		/* pressure acceleration */
		/*  NOTE: the '-' is important for the correctness of the pressure
			computation. Without the '-' we need to compute 'part_i_to_j'
			as pos_j - pos_i. */
		__pm3_sub_v_v(part_i_to_j, ps[i].cur_pos, ps[j].cur_pos);
		float Pij = -ps[j].mass*(
			ps[i].pressure*__pm_inv(__pm_sq(ps[i].density)) +
			ps[j].pressure*__pm_inv(__pm_sq(ps[j].density))
		);
		kernel_pressure(part_i_to_j, d2, pressure_dir);
		__pm3_assign_vs(press_acc, pressure_dir,Pij);
		__pm3_add_acc_v(acc, press_acc);

#if OUTPUT_ACCEL_PRESS == 1
		cout << "        Pij= " << Pij << endl;
		cout << "            ps[" << i << "].pressure = " << ps[i].pressure << endl;
		cout << "            ps[" << i << "].density  = " << ps[i].density << endl;
		cout << "            ps[" << j << "].pressure = " << ps[j].pressure << endl;
		cout << "            ps[" << j << "].density  = " << ps[j].density << endl;
		cout << "            ps[" << j << "].mass     = " << ps[j].mass << endl;
		cout << "            kernel result: " << __pm3_out(pressure_dir) << endl;
		cout << "        pressure vector: " << __pm3_out(press_acc) << endl;
#endif

		/* viscosity acceleration */
		float Vij = viscosity*ps[j].mass*__pm_inv(ps[i].density*ps[j].density);
		Vij *= kernel_viscosity(d2);
		__pm3_sub_v_v_mul_s(visc_acc, ps[j].cur_vel,ps[i].cur_vel, Vij);
		__pm3_add_acc_v(acc, visc_acc);

#if OUTPUT_ACCEL_VISC == 1
		cout << "        Vij= " << Vij << endl;
		cout << "            ps[" << i << "].density= " << ps[i].density << endl;
		cout << "            ps[" << j << "].density= " << ps[j].density << endl;
		cout << "            ps[" << j << "].mass= " << ps[j].mass << endl;
		cout << "            1/(rho_i*tho_j)= " << inv(ps[i].density*ps[j].density) << endl;
		cout << "            kernel result= " << kernel_viscosity(d2) << endl;
		cout << "            ps[" << i << "].cur_vel= " << __pm3_out(ps[i].cur_vel) << endl;
		cout << "            ps[" << j << "].cur_vel= " << __pm3_out(ps[j].cur_vel) << endl;
		vec3 vel_diff;
		__pm3_sub_v_v(vel_diff, ps[j].cur_vel,ps[i].cur_vel);
		cout << "            vel diff= " << __pm3_out(vel_diff) << endl;
		cout << "        viscosity vector= " << __pm3_out(visc_acc) << endl;
#endif
	}

	/* compute force in particle*/
	__pm3_assign_vs(ps[i].force, acc, ps[i].mass);

#if OUTPUT_FORCE == 1
	cout << "    total force= " << __pm3_out(ps[i].force) << endl;
#endif
}

// PUBLIC

newtonian::newtonian() : fluid() {
}

newtonian::~newtonian() {
}

// MODIFIERS

void newtonian::update_forces() {
#if (OUTPUT > 0)
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
#endif

	// Compute neighbour lists, and squared distances
	// between a particle and its neighbours.
	// We need a dynamic list for the neighbours, but a
	// fixed-size list of these lists will suffice.
	vector<vector<size_t> > all_neighs(N);
	vector<vector<float> > all_d2s(N);

#if SAFE_NEIGH == 1
	for (size_t i = 0; i < N; ++i) {
		make_neighbours_lists(i, all_neighs[i], all_d2s[i]);
	}
#endif

#if EXPL_NEIGH == 1
	make_partition();
	for (size_t i = 0; i < N; ++i) {
		make_neighbours_lists_tree(i, all_neighs[i], all_d2s[i]);
	}
#endif

#if OUTPUT > 0
	cout << "Density and pressure" << endl;
#endif

	// compute density and pressure of each particle
	for (size_t i = 0; i < N; ++i) {
		initialise_density_pressure(i, all_neighs[i], all_d2s[i]);
	}

#if OUTPUT_ACCEL_FORCE > 0
	cout << "Force" << endl;
#endif

	// compute forces of the fluid (due to pressure and viscosity)
	for (size_t i = 0; i < N; ++i) {
		update_force(i, all_neighs[i], all_d2s[i]);
	}

#if OUTPUT > 0
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
#endif
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

#if SAFE_NEIGH == 1
	#pragma omp parallel for num_threads(n)
	for (size_t i = 0; i < N; ++i) {
		make_neighbours_lists(i, all_neighs[i], all_d2s[i]);
	}
#endif

#if EXPL_NEIGH == 1
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

