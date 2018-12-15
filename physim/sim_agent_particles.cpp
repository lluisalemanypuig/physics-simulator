#include <physim/simulator.hpp>

// physim includes
#include <physim/math/private/math3.hpp>
#include <physim/particles/conversions.hpp>
#include <physim/sim_solver.cpp>

namespace physim {
using namespace particles;

void simulator::_simulate_agent_particles() {

	for (agent_particle *p : aps) {
		// ignore fixed particles
		if (p->fixed) {
			continue;
		}

		/*
		// Reset a particle when it dies.
		// Do not smiulate this particle
		// until the next step
		if (p->lifetime <= 0.0f) {
			init_particle(p);
			continue;
		}
		// is this particle allowed to move?
		// if not, ignore it
		p->reduce_starttime(dt);
		if (p->starttime > 0.0f) {
			continue;
		}

		// Particles age: reduce their lifetime.
		p->reduce_lifetime(dt);
		*/

		// clear the current force
		__pm3_assign_s(p->force, 0.0f);
		// compute forces for particle p
		compute_forces(p);

		// apply solver to predict next position and
		// velocity of the particle
		math::vec3 pred_pos, pred_vel;
		apply_solver(p, pred_pos, pred_vel);

		// collision prediction:
		// copy the particle at its current state and use it
		// to predict the update upon collision with geometry
		sized_particle coll_pred;

		// check if there is any collision between
		// this free particle and a geometrical object

		bool collision =
		find_update_geom_collision_sized(p, pred_pos, pred_vel, coll_pred);

		// give the particle the proper final state
		if (collision) {
			//*p = coll_pred;
			from_sized_to_agent(coll_pred, *p);
		}
		else {
			p->save_position();
			__pm3_assign_v(p->cur_pos, pred_pos);
			__pm3_assign_v(p->cur_vel, pred_vel);
		}

		// Now it is time to perform collisions between particles.
		// A data structure would come really in handy but, for now,
		// a linear-time algorithm (for every particle) will be used.

		if (part_part_colls_activated()) {
			find_update_particle_collision_sized(p, i);
		}
	}

}

} // -- namespace physim
