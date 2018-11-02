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
	// clear the current force
	__pm_assign_s(p->force, 0.0f);

	// compute the force every force field
	// makes on every particle
	math::vec3 F;
	for (fields::field *f : force_fields) {
		f->compute_force(p, F);
		__pm_add_acc_v(p->force, F);
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

		if (g->intersec_segment(p->cur_pos, pred_pos)) {
			collision = true;

			coll_pred = *p;

			// the geometry updates the predicted particle
			g->update_particle(pred_pos, pred_vel, &coll_pred);

			if (solver == solver_type::Verlet) {
				// this solver needs a correct position
				// for the 'previous' position of the
				// particle after a collision with geometry

				__pm_sub_v_vs(coll_pred.prev_pos, coll_pred.cur_pos, coll_pred.cur_vel, dt);
			}

			// keep track of the predicted particle's position
			__pm_assign_v(pred_pos, coll_pred.cur_pos);
			__pm_assign_v(pred_vel, coll_pred.cur_vel);
		}
	}

	// give the particle the proper final state
	if (collision) {
		*p = coll_pred;
	}
	else {
		p->save_position();
		__pm_assign_v(p->cur_pos, pred_pos);
		__pm_assign_v(p->cur_vel, pred_vel);
	}
}

} // -- namespace physim
