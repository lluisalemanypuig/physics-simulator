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

		/* ------------------ */
		/*   compute forces   */

		// clear the current force
		__pm3_assign_s(p->force, 0.0f);

		// particle agents are not affected by force fields -.-'

		// compute steering force
		__pm3_sub_v_v(p->force, p->target, p->cur_pos);
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

		/* ------------------- */
		/* STEERING BEHAVIOURS */

		vec3 desired_vel;
		vec3 steer_dir;

		vec3 target_offset;
		float dist_target, clipped_speed;

		switch (p->behaviour) {

		/* seek */
		case agent_behaviour_type::seek:
			// unit velocity vector towards target
			__pm3_sub_v_v(desired_vel, p->target, p->cur_pos);
			normalise(desired_vel, desired_vel);
			// make velocity have magnitude equal to max_speed
			__pm3_mul_acc_s(desired_vel, p->max_speed);
			// modify agent's velocity
			__pm3_sub_v_v(steer_dir, desired_vel, p->cur_vel);
			__pm3_add_acc_v(p->cur_vel, steer_dir);
			break;

		/* flee */
		case agent_behaviour_type::flee:
			// unit velocity vector towards target
			__pm3_sub_v_v(desired_vel, p->cur_pos, p->target);
			normalise(desired_vel, desired_vel);
			// make velocity have magnitude equal to max_speed
			__pm3_mul_acc_s(desired_vel, p->max_speed);
			// modify agent's velocity
			__pm3_sub_v_v(steer_dir, desired_vel, p->cur_vel);
			__pm3_add_acc_v(p->cur_vel, steer_dir);
			break;

		/* arrival */
		case agent_behaviour_type::arrival:
			// unit velocity vector towards target
			__pm3_sub_v_v(target_offset, p->target, p->cur_pos);
			dist_target = __pm3_norm(target_offset);
			clipped_speed = std::min
			(p->max_speed*dist_target/p->slowing_distance,p->max_speed);
			__pm3_assign_vs(desired_vel, target_offset, clipped_speed/dist_target);
			__pm3_sub_v_v(steer_dir, desired_vel, p->cur_vel);
			__pm3_add_acc_v(p->cur_vel, steer_dir);
			break;

		default:
			;
		}

		// finally, truncate velocity again
		truncate(p->cur_vel, p->max_speed, p->cur_vel);
	}

}

} // -- namespace physim
