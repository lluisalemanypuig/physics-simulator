#include <physim/simulator.hpp>

// C++ includes
#include <iostream>
using namespace std;

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

		/* ------------------ */
		/*   compute forces   */

		// clear the current force
		__pm3_assign_s(p->force, 0.0f);

		// particle agents are not affected by force fields -.-'

		// compute steering force
		__pm3_sub_v_v(p->force, p->attractor, p->cur_pos);
		truncate(p->force, p->max_force, p->force);

		/* -------------------------- */
		/*   apply Euler integration  */
		/*  to calculate new velocity */
		/*         and position       */

		vec3 pred_vel, pred_pos;
		// compute new velocity
		__pm3_add_v_vs(pred_vel, p->cur_vel, p->force, dt/p->mass);
		truncate(pred_vel, p->max_speed, pred_vel);
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

		// here we should compute a bit more steering:

		/* seek steering */
		vec3 desired_vel;
		__pm3_sub_v_v(desired_vel, p->attractor, p->cur_pos);
		normalise(desired_vel, desired_vel);
		__pm3_mul_acc_s(desired_vel, p->max_speed);
		vec3 steer_dir;
		__pm3_sub_v_v(steer_dir, desired_vel, p->cur_vel);

		/* modify agent's velocity */
		__pm3_add_acc_v(p->cur_vel, steer_dir);

		// finally, truncate velocity again
		truncate(p->cur_vel, p->max_speed, p->cur_vel);
	}

}

} // -- namespace physim
