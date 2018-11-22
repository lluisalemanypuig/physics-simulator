#include <physim/simulator.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/fields/gravitational_planet.hpp>
#include <physim/math/private/math3.hpp>
#include <physim/sim_solver.cpp>

namespace physim {

void simulator::_simulate_sized_particles() {

	for (particles::sized_particle *p : sps) {
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

		// check if there is any collision between
		// this free particle and a geometrical object

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

		// collision prediction:
		// copy the particle at its current state and use it
		// to predict the update upon collision with geometry
		particles::sized_particle coll_pred;

		// has there been any collision?
		bool collision = false;

		// Check collision between the particle and
		// every fixed geometrical object in the scene.

		for (unsigned int i = 0; i < scene_fixed.size(); ++i) {
			const geom::geometry *g = scene_fixed[i];

			// if the particle collides with some geometry
			// then the geometry is in charge of updating
			// this particle's position, velocity, ...

			bool inter = false;
			inter = inter or g->intersec_segment(p->cur_pos, pred_pos);
			inter = inter or g->intersec_sphere(pred_pos, p->R);
			if (inter) {
				collision = true;

				coll_pred = *p;

				// the geometry updates the predicted particle
				g->update_particle(pred_pos, pred_vel, &coll_pred);

				if (solver == solver_type::Verlet) {
					// this solver needs a correct position
					// for the 'previous' position of the
					// particle after a collision with geometry

					__pm3_sub_v_vs(coll_pred.prev_pos, coll_pred.cur_pos, coll_pred.cur_vel, dt);
				}

				// keep track of the predicted particle's position
				__pm3_assign_v(pred_pos, coll_pred.cur_pos);
				__pm3_assign_v(pred_vel, coll_pred.cur_vel);
			}
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
