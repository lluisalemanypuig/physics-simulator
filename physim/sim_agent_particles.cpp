#include <physim/simulator.hpp>

// physim includes
#include <physim/math/private/math3.hpp>
#include <physim/particles/conversions.hpp>
#include <physim/sim_solver.cpp>

namespace physim {
using namespace particles;
using namespace math;

void simulator::_simulate_agent_particles() {

	// first compute forces ...
	for (size_t i = 0; i < aps.size(); ++i) {
		agent_particle& p = aps[i];

		// ignore fixed particles
		if (p.fixed) {
			continue;
		}

		// Particles age: reduce their lifetime.
		p.reduce_lifetime(dt);

		/* ------------------- */
		/* STEERING BEHAVIOURS */

		vec3 steer_force;
		__pm3_assign_s(steer_force, 0.0f);

		p.apply_behaviours(steer_force);
		p.apply_behaviours(scene_fixed, steer_force);
		p.apply_behaviours(aps, steer_force);

		// store force
		truncate(steer_force, p.max_force, p.force);
	}

	// ... then update velocities and positions
	for (size_t i = 0; i < aps.size(); ++i) {
		agent_particle& p = aps[i];

		// ignore fixed particles
		if (p.fixed) {
			continue;
		}

		vec3 accel;
		__pm3_div_v_s(accel, p.force, p.mass);

		vec3 pred_vel;
		__pm3_add_v_vs(pred_vel, p.cur_vel, accel, dt);

		/* ------------------------- */
		/* apply Euler integration   */
		/* to calculate new velocity */
		/* and position              */

		vec3 pred_pos;
		// compute new position
		__pm3_add_v_vs(pred_pos, p.cur_pos, pred_vel, dt);

		/* --------------------------- */
		/*   apply Euler integration   */

		// collision prediction:
		// copy the particle at its current state and use it
		// to predict the update upon collision with geometry
		agent_particle coll_pred = p;

		// check if there is any collision between
		// this free particle and a geometrical object

		bool collision =
		find_update_geomcoll_sized(p, pred_pos, pred_vel, coll_pred);

		// give the particle the proper final state
		if (collision) {
			p = coll_pred;
		}
		else {
			p.save_position();
			__pm3_assign_v(p.cur_pos, pred_pos);
			__pm3_assign_v(p.cur_vel, pred_vel);
		}

		// Now it is time to perform collisions between particles.
		// A data structure would come really in handy but, for now,
		// a linear-time algorithm (for every particle) will be used.

		if (part_part_colls_activated()) {
			find_update_partcoll_agent(p, i);
		}

		// compute new orientation
		vec3 alignment;
		__pm3_sub_v_v(alignment, p.cur_vel, p.orientation);
		normalise(alignment, alignment);
		__pm3_add_acc_vs(p.orientation, alignment, p.align_weight);
		normalise(p.orientation, p.orientation);
	}

}

} // -- namespace physim
