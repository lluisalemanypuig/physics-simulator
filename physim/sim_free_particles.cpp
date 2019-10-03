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
#include <omp.h>

// physim includes
#include <physim/math/private/math3.hpp>
#include <physim/sim_solver.cpp>

namespace physim {
using namespace particles;
using namespace math;

void simulator::_simulate_free_particles() {
	for (free_particle& p : fps) {
		// ignore fixed particles
		if (p.fixed) {
			continue;
		}
		// Reset a particle when it dies.
		// Do not smiulate this particle
		// until the next step
		if (p.lifetime <= 0.0f) {
			init_particle(p);
			continue;
		}
		// is this particle allowed to move?
		// if not, ignore it
		p.reduce_starttime(dt);
		if (p.starttime > 0.0f) {
			continue;
		}

		// clear the current force
		__pm3_assign_s(p.force, 0.0f);
		// compute forces for particle p
		compute_forces(p);

		// Particles age: reduce their lifetime.
		p.reduce_lifetime(dt);

		// apply solver to predict next position and
		// velocity of the particle
		vec3 pred_pos, pred_vel;
		apply_solver(p, pred_pos, pred_vel);

		// collision prediction:
		// copy the particle at its current state and use it
		// to predict the update upon collision with geometry
		free_particle coll_pred;

		// check if there is any collision between
		// this free particle and a geometrical object

		bool collision =
		find_update_geomcoll_free(p, pred_pos, pred_vel, coll_pred);

		if (part_part_colls_activated()) {
			bool r = find_update_partcoll_free
			(p, pred_pos, pred_vel, coll_pred);

			collision = collision or r;
		}

		// give the particle the proper final state
		if (collision) {
			p = coll_pred;
		}
		else {
			p.save_position();
			__pm3_assign_v(p.cur_pos, pred_pos);
			__pm3_assign_v(p.cur_vel, pred_vel);
		}
	}
}

} // -- namespace physim
