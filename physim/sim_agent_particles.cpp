#include <physim/simulator.hpp>

// physim includes
#include <physim/math/private/math3.hpp>
#include <physim/particles/conversions.hpp>
#include <physim/sim_solver.cpp>

namespace physim {
using namespace particles;
using namespace math;

void simulator::_simulate_agent_particles() {

	for (size_t i = 0; i < aps.size(); ++i) {
		agent_particle *p = aps[i];

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
		*/

		// Particles age: reduce their lifetime.
		p->reduce_lifetime(dt);

		/* ------------------- */
		/* STEERING BEHAVIOURS */

		vec3 steer_force;
		__pm3_assign_s(steer_force, 0.0f);

		p->apply_behaviours(steer_force);
		for (const geometric::geometry *g : scene_fixed) {
			p->apply_behaviours(g, steer_force);
		}

		truncate(steer_force, p->max_force, steer_force);
		vec3 accel;
		__pm3_div_v_s(accel, steer_force, p->mass);

		vec3 pred_vel;
		__pm3_add_v_vs(pred_vel, p->cur_vel, accel, dt);

		/* ------------------------- */
		/* apply Euler integration   */
		/* to calculate new velocity */
		/* and position              */

		vec3 pred_pos;
		// compute new position
		__pm3_add_v_vs(pred_pos, p->cur_pos, pred_vel, dt);

		/* --------------------------- */
		/*   apply Euler integration   */

		// collision prediction:
		// copy the particle at its current state and use it
		// to predict the update upon collision with geometry
		agent_particle coll_pred = *p;

		// check if there is any collision between
		// this free particle and a geometrical object

		bool collision =
		find_update_geomcoll_sized(p, pred_pos, pred_vel, coll_pred);

		// give the particle the proper final state
		if (collision) {
			*p = coll_pred;
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
			find_update_partcoll_agent(p, i);
		}
	}

}

} // -- namespace physim
