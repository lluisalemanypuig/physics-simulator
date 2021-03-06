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

#include <physim/simulator.hpp>

// C includes
#include <assert.h>
#include <omp.h>

// physim includes
#include <physim/particles/conversions.hpp>
#include <physim/math/private/math3.hpp>
#include <physim/fluids/fluid.hpp>
#include <physim/sim_solver.cpp>

namespace physim {
using namespace particles;
using namespace fluids;
using namespace math;

void simulator::_simulate_fluids() {

	// collision prediction:
	// copy the particle at its current state and use it
	// to predict the update upon collision with geometry.
	// Although it is a free particle, the attributes
	// of the mesh particle will be copied into this one.
	free_particle current;
	free_particle coll_pred;
	current.bouncing = 0.1f;
	coll_pred.bouncing = 0.1f;

	for (fluid *f : fs) {

		current.friction = f->get_viscosity()/50000.0f;
		coll_pred.friction = f->get_viscosity()/50000.0f;

		/* update a meshe's particles */
		fluid_particle *fluid_ps = f->get_particles();
		size_t N = f->size();

		// compute forces for particle p that are
		// originated within the mesh's structure
		f->update_forces();

		for (size_t p_idx = 0; p_idx < N; ++p_idx) {

			// compute forces for particle p that are
			// originated by the force fields of the
			// simulation
			compute_forces(fluid_ps[p_idx]);

			// apply solver to predict next position and
			// velocity of the particle
			vec3 pred_pos, pred_vel;
			apply_solver(fluid_ps[p_idx], pred_pos, pred_vel);

			// check if there is any collision between
			// this mesh particle and a geometrical object

			/* The algorithm for updating a particle's position
			 * and velocity is the same as for a free particle.
			 * The only change here is the call to the appropriate
			 * function in the geometry class for a meshe's particle
			 * update.
			 */

			from_fluid_to_free(fluid_ps[p_idx], current);
			from_fluid_to_free(fluid_ps[p_idx], coll_pred);

			bool collision =
			find_update_geomcoll_free(current, pred_pos, pred_vel, coll_pred);

			if (part_part_colls_activated()) {
				bool r = find_update_partcoll_free
				(current, pred_pos, pred_vel, coll_pred);

				collision = collision or r;
			}

			// give the particle the proper final state
			if (collision) {
				from_free_to_fluid(coll_pred, fluid_ps[p_idx]);
			}
			else {
				fluid_ps[p_idx].save_position();
				__pm3_assign_v(fluid_ps[p_idx].cur_pos, pred_pos);
				__pm3_assign_v(fluid_ps[p_idx].cur_vel, pred_vel);
			}

			// clear the force so that in the next iteration
			// for the mesh, the forces can be computed
			__pm3_assign_s(fluid_ps[p_idx].force, 0.0f);
		}
	}
}

void simulator::_simulate_fluids(size_t n) {

	// collision prediction:
	// copy the particle at its current state and use it
	// to predict the update upon collision with geometry.
	// Although it is a free particle, the attributes
	// of the mesh particle will be copied into this one.
	free_particle current;
	free_particle coll_pred;

	for (fluid *f : fs) {

		current.friction = f->get_viscosity()/50000.0f;
		coll_pred.friction = f->get_viscosity()/50000.0f;

		/* update a meshe's particles */
		fluid_particle *fluid_ps = f->get_particles();
		size_t N = f->size();

		// compute forces for particle p that are
		// originated within the mesh's structure
		f->update_forces(n);

		for (size_t p_idx = 0; p_idx < N; ++p_idx) {
			current.bouncing = 0.1f;
			coll_pred.bouncing = 0.1f;

			// compute forces for particle p that are
			// originated by the force fields of the
			// simulation
			compute_forces(fluid_ps[p_idx]);

			// apply solver to predict next position and
			// velocity of the particle
			vec3 pred_pos, pred_vel;
			apply_solver(fluid_ps[p_idx], pred_pos, pred_vel);

			// check if there is any collision between
			// this mesh particle and a geometrical object

			/* The algorithm for updating a particle's position
			 * and velocity is the same as for a free particle.
			 * The only change here is the call to the appropriate
			 * function in the geometry class for a meshe's particle
			 * update.
			 */

			from_fluid_to_free(fluid_ps[p_idx], current);
			from_fluid_to_free(fluid_ps[p_idx], coll_pred);

			bool collision =
			find_update_geomcoll_free(current, pred_pos, pred_vel, coll_pred);

			if (part_part_colls_activated()) {
				bool r = find_update_partcoll_free
				(current, pred_pos, pred_vel, coll_pred);

				collision = collision or r;
			}

			// give the particle the proper final state
			if (collision) {
				from_free_to_fluid(coll_pred, fluid_ps[p_idx]);
			}
			else {
				fluid_ps[p_idx].save_position();
				__pm3_assign_v(fluid_ps[p_idx].cur_pos, pred_pos);
				__pm3_assign_v(fluid_ps[p_idx].cur_vel, pred_vel);
			}

			// clear the force so that in the next iteration
			// for the mesh, the forces can be computed
			__pm3_assign_s(fluid_ps[p_idx].force, 0.0f);
		}
	}
}

} // -- namespace physim

