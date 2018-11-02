#include <physim/simulator.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/fields/gravitational_planet.hpp>
#include <physim/math/math.hpp>
#include <physim/sim_solver.cpp>

namespace physim {

void simulator::simulate_meshes() {

	for (meshes::mesh *m : ms) {

		/* check mesh start time and lifetime
		 * of mesh
		 */

		/* update a meshe's particles */
		particles::mesh_particle **mps = m->get_particles();
		size_t N = m->size();

		// set forces to 0
		for (size_t i = 0; i < N; ++i) {
			__pm_assign_s(mps[i]->force, 0.0f);
		}

		// compute forces for particle p that are
		// originated within the mesh's structure
		m->update_forces();

		for (size_t i = 0; i < N; ++i) {
			// ignore fixed particles
			if (mps[i]->fixed) {
				continue;
			}

			// compute forces for particle p that are
			// originated by the force fields of the
			// simulation
			compute_forces(mps[i]);

			// apply solver to predict next position and
			// velocity of the particle
			math::vec3 pred_pos, pred_vel;
			apply_solver(mps[i], pred_pos, pred_vel);

			// check if there is any collision between
			// this mesh particle and a geometrical object

			/* The algorithm for updating a particle's position
			 * and velocity is the same as for a free particle.
			 * The only change here is the call to the appropriate
			 * function in the geometry class for a meshe's particle
			 * update.
			 */

			// collision prediction:
			// copy the particle at its current state and use it
			// to predict the update upon collision with geometry
			particles::mesh_particle coll_pred;

			// has there been any collision?
			bool collision = false;

			// Check collision between the particle and
			// every fixed geometrical object in the scene.

			for (unsigned int i = 0; i < scene_fixed.size(); ++i) {
				const geom::geometry *g = scene_fixed[i];

				// if the particle collides with some geometry
				// then the geometry is in charge of updating
				// this particle's position, velocity, ...

				if (g->intersec_segment(mps[i]->cur_pos, pred_pos)) {
					collision = true;

					coll_pred = *mps[i];

					// the geometry updates the predicted particle
					g->update_particle(pred_pos, pred_vel, i, m);

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
				*mps[i] = coll_pred;
			}
			else {
				mps[i]->save_position();
				__pm_assign_v(mps[i]->cur_pos, pred_pos);
				__pm_assign_v(mps[i]->cur_vel, pred_vel);
			}

			// clear the force so that in the next iteration
			// for the mesh, the forces can be computed
			__pm_assign_s(mps[i]->force, 0.0f);
		}
	}
}

} // -- namespace physim

