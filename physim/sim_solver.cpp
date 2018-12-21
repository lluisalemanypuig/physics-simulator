
/* This file contains small functions that can be used for
 * all types of particles.
 *
 * This file is for library's internal use only.
 * Do not include.
 */

// physim includes
#include <physim/simulator.hpp>
#include <physim/math/vec3.hpp>
#include <physim/math/private/math3.hpp>

// C++ includes
#include <iostream>
#include <vector>

namespace physim {

template<class P>
void simulator::apply_solver(const P *p, math::vec3& pred_pos, math::vec3& pred_vel) {
	const float mass = p->mass;

	switch (solver) {
		case solver_type::EulerOrig:
			// pred_pos <- pos + vel*dt
			__pm3_add_v_vs(pred_pos, p->cur_pos, p->cur_vel, dt);
			// pred_vel <- vel + force*dt/mass
			__pm3_add_v_vs(pred_vel, p->cur_vel, p->force, dt*(1.0f/mass));
			break;

		case solver_type::EulerSemi:
			// pred_vel <- vel + force*dt/mass
			__pm3_add_v_vs(pred_vel, p->cur_vel, p->force, dt*(1.0f/mass));
			// pred_pos <- pos + pred_vel*dt
			__pm3_add_v_vs(pred_pos, p->cur_pos, pred_vel, dt);
			break;

		case solver_type::Verlet:
			// pred_pos <- pos + 1.0f*(pos - prev_pos) + force*dt*dt/m
			__pm3_sub_v_v(pred_pos, p->cur_pos, p->prev_pos);
			__pm3_add_vs_vs_v(pred_pos, pred_pos,1.0f, p->force,(dt*dt)*(1.0f/mass), p->cur_pos);

			// pred_vel <- (pred_pos - pos)/dt
			__pm3_sub_v_v_div_s(pred_vel, pred_pos, p->cur_pos, dt);
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
		__pm3_add_acc_v(p->force, F);
	}

	// apply viscous drag
	__pm3_add_acc_vs(p->force, p->cur_vel, -visc_drag);
}

} // -- namespace physim
