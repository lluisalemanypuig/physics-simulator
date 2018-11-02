#pragma once

/* This file contains a small function that tries to
 * update a particle after a collision with geometry
 * as best as possible (without using any data structure).
 *
 * This function tries to take into account that a
 * particle may collide with several geometrical objects
 * at the same time.
 *
 * This file is for the library's internal use only.
 * Do not include.
 */

// C++ includes
#include <vector>

// physim includes
#include <physim/simulator.hpp>
#include <physim/geometry/geometry.hpp>
#include <physim/math/vec3.hpp>
#include <physim/math/math.hpp>

namespace physim {

template<class P>
void simulator::apply_solver(const P *p, math::vec3& pred_pos, math::vec3& pred_vel) {
	const float mass = p->mass;

	switch (solver) {
		case solver_type::EulerOrig:
			// pred_pos <- pos + vel*dt
			__pm_add_v_vs(pred_pos, p->cur_pos, p->cur_vel, dt);
			// pred_vel <- vel + force*dt/mass
			__pm_add_v_vs(pred_vel, p->cur_vel, p->force, dt/mass);
			break;

		case solver_type::EulerSemi:
			// pred_vel <- vel + force*dt/mass
			__pm_add_v_vs(pred_vel, p->cur_vel, p->force, dt/mass);
			// pred_pos <- pos + pred_vel*dt
			__pm_add_v_vs(pred_pos, p->cur_pos, pred_vel, dt);
			break;

		case solver_type::Verlet:
			// pred_pos <- pos + 1.0f*(pos - prev_pos) + force*dt*dt/m
			__pm_sub_v_v(pred_pos, p->cur_pos, p->prev_pos);
			__pm_add_vs_vs_v(pred_pos, pred_pos,1.0f, p->force,(dt*dt)*(1.0f/mass), p->cur_pos);

			// pred_vel <- (pred_pos - pos)/dt
			__pm_sub_v_v_div_s(pred_vel, pred_pos, p->cur_pos, dt);
			break;

		default:
			std::cerr << "Warning: solver not implemented" << std::endl;
	}
}

template<class P>
void simulator::compute_forces(P *p) {
	// compute the force every force field
	// makes on every particle
	math::vec3 F;
	for (fields::field *f : force_fields) {
		f->compute_force(p, F);
		__pm_add_acc_v(p->force, F);
	}
}

} // -- namespace physim
