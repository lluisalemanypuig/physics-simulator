#include <physim/simulator.hpp>

// physim includes
#include <physim/math/private/math3.hpp>
#include <physim/sim_solver.cpp>

namespace physim {

void simulator::_simulate_sized_particles() {

	for (size_t i = 0; i < sps.size(); ++i) {
		particles::sized_particle *p = sps[i];

		// ignore fixed particles
		if (p->fixed) {
			continue;
		}
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

		// clear the current force
		__pm3_assign_s(p->force, 0.0f);
		// compute forces for particle p
		compute_forces(p);

		// Particles age: reduce their lifetime.
		p->reduce_lifetime(dt);

		// apply solver to predict next position and
		// velocity of the particle
		math::vec3 pred_pos, pred_vel;
		apply_solver(p, pred_pos, pred_vel);

		// collision prediction:
		// copy the particle at its current state and use it
		// to predict the update upon collision with geometry
		particles::sized_particle coll_pred;

		// check if there is any collision between
		// this sized particle and a geometrical object

		bool collision =
		find_update_geom_collision_sized(p, pred_pos, pred_vel, coll_pred);

		// Now it is time to perform collisions between particles.
		// A data structure would come really in handy but, for now,
		// a linear-time algorithm (for every particle) will be used.

		if (part_part_colls_activated()) {
			bool r = find_update_particle_collision_sized
			(p, i, pred_pos, pred_vel, coll_pred);

			collision = collision or r;
		}

		// give the particle the proper final state
		if (collision) {
			*p = coll_pred;
		}
		else {
			p->save_position();
			__pm3_assign_v(p->cur_pos, pred_pos);
			__pm3_assign_v(p->cur_vel, pred_vel);
		}
	}

}

} // -- namespace physim
