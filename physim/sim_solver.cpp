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
	const float mass = p->get_mass();

	switch (solver) {
		case solver_type::EulerOrig:
			// pred_pos <- pos + vel*dt
			__pm_add_v_vs(pred_pos, p->get_position(), p->get_velocity(), dt);
			// pred_vel <- vel + force*dt/mass
			__pm_add_v_vs(pred_vel, p->get_velocity(), p->get_force(), dt/mass);
			break;

		case solver_type::EulerSemi:
			// pred_vel <- vel + force*dt/mass
			__pm_add_v_vs(pred_vel, p->get_velocity(), p->get_force(), dt/mass);
			// pred_pos <- pos + pred_vel*dt
			__pm_add_v_vs(pred_pos, p->get_position(), pred_vel, dt);
			break;

		case solver_type::Verlet:
			// pred_pos <- pos + 1.0f*(pos - prev_pos) + force*dt*dt/m
			__pm_sub_v_v(pred_pos, p->get_position(), p->get_previous_position());
			__pm_add_vs_vs_v(pred_pos, pred_pos,1.0f, p->get_force(),(dt*dt)*(1.0f/mass), p->get_position());

			// pred_vel <- (pred_pos - pos)/dt
			__pm_sub_v_v_div_s(pred_vel, pred_pos, p->get_position(), dt);
			break;

		default:
			std::cerr << "Warning: solver not implemented" << std::endl;
	}
}

template<class P>
void simulator::compute_forces(P *p) {
	// clear the current force
	__pm_assign_s(p->get_force(), 0.0f);

	// compute the force every force field
	// makes on every particle
	math::vec3 F;
	for (fields::field *f : force_fields) {
		f->compute_force(p, F);
		__pm_add_acc_v(p->get_force(), F);
	}
}

template<class P>
void simulator::check_collision_update
(
	math::vec3& pred_pos, math::vec3& pred_vel,
	P *p
)
{
	/* The main idea implemented in the following loop
	 * is the following:
	 *  -> We now know a prediction of the next position
	 *     of the particle thanks to the solver. This is
	 *     the "predicted position", stored in variable
	 *     'pred_pos'.
	 *
	 *  -> Now we have a segment ('prev_pos','pred_pos')
	 *     with which we can check collision with other
	 *     geometry.
	 *
	 *  -> However, we should not update the particle with
	 *     the first collision that has been detected.
	 *     Instead, a prediction of the result of the
	 *     collision is obtained and stored in 'pred_particle'.
	 *     This particle is called the "predicted particle".
	 *     Notice that this predicted particle also contains
	 *     the predicted velocity, and may have other attributes
	 *     modified after the collision (as part of the result
	 *	   of the collision).
	 *
	 *  -> Then, checking the collision is done with the
	 *     new segment from prev_pos to the position of
	 *     the predicted particle. The position of the
	 *     predicted particle, when there is any collision,
	 *     is stored in variable 'pred_pos'.
	 *
	 *  -> Therefore, after the first collision, we have
	 *     a prediction of the result of that collision
	 *     of the particle with some geometry. Then, the
	 *     second collision is detected with the segment
	 *     from the previous position ('prev_pos') and
	 *     the position of the predicted particle ('pred_pos').
	 *     However, be aware now that the result of the
	 *     second collision is NOT done with the state
	 *     of the predicted particle. Instead, it is done
	 *     with the original state of the particle.
	 *
	 *  -> The state that the particle takes is the state
	 *     of the predicted particle at the end of the loop
	 *     (only if there had been any collision).
	 *
	 */

	// collision prediction
	// copy the particle at its current state and use it
	// to predict the update upon collision with geometry
	P coll_pred;

	// has there been any collision?
	bool collision = false;

	// Check collision between the particle and
	// every fixed geometrical object in the scene.

	for (unsigned int i = 0; i < scene_fixed.size(); ++i) {
		const geom::geometry *g = scene_fixed[i];

		// if the particle collides with some geometry
		// then the geometry is in charge of updating
		// this particle's position, velocity, ...

		if (g->intersec_segment(p->get_position(), pred_pos)) {
			collision = true;

			coll_pred = *p;

			// the geometry updates the predicted particle
			g->update_particle(pred_pos, pred_vel, &coll_pred);

			if (solver == solver_type::Verlet) {
				// this solver needs a correct position
				// for the 'previous' position of the
				// particle after a collision with geometry

				math::vec3& cp_prev_pos = coll_pred.get_previous_position();
				__pm_sub_v_vs(cp_prev_pos, coll_pred.get_position(), coll_pred.get_velocity(), dt);
			}

			// keep track of the predicted particle's position
			__pm_assign_v(pred_pos, coll_pred.get_position());
			__pm_assign_v(pred_vel, coll_pred.get_velocity());
		}
	}

	// give the particle the proper final state
	if (collision) {
		*p = coll_pred;
	}
	else {
		p->save_position();
		p->set_position(pred_pos);
		p->set_velocity(pred_vel);
	}
}

} // -- namespace physim
