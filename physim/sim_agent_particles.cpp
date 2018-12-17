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

	vec3 dir;

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

		// Particles age: reduce their lifetime.
		p->reduce_lifetime(dt);
		*/

		// clear the current force
		__pm3_assign_s(p->force, 0.0f);
		// compute forces for particle p,
		// using fields and others
		compute_forces(p);
		// compute force from its attractor
		__pm3_sub_v_v(dir, p->attractor, p->cur_pos);
		normalise(dir,dir);
		float extra = std::sin(__pm3_angle(dir, p->cur_vel)) + 4.0f;
		__pm3_add_acc_vs(p->force, dir, p->attractor_acceleration*extra);

		// apply solver to predict next position and
		// velocity of the particle
		vec3 pred_pos, pred_vel;
		apply_solver(p, pred_pos, pred_vel);

		// scale velocity down to max_vel
		float speed2 = __pm3_norm2(pred_vel);
		if (speed2 >= p->desired_vel*p->desired_vel) {
			__pm3_assign_vs(pred_vel, pred_vel, p->desired_vel/std::sqrt(speed2));
		}

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
