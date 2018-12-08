#include <physim/simulator.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/fields/gravitational_planet.hpp>
#include <physim/particles/conversions.hpp>
#include <physim/math/private/math3.hpp>
#include <physim/sim_solver.cpp>

namespace physim {

void simulator::_simulate_meshes() {

	// collision prediction:
	// copy the particle at its current state and use it
	// to predict the update upon collision with geometry.
	// Although it is a free particle, the attributes
	// of the mesh particle will be copied into this one.
	particles::free_particle coll_pred;

	for (meshes::mesh *m : ms) {

		/* check mesh start time and lifetime
		 * of mesh
		 */

		// some of the meshe's attributes are needed
		// in the collision prediction particle for...
		// collision prediction
		coll_pred.friction = m->get_friction();
		coll_pred.bouncing = m->get_bouncing();

		/* update a meshe's particles */
		particles::mesh_particle **mps = m->get_particles();
		size_t N = m->size();

		// set forces to 0
		for (size_t i = 0; i < N; ++i) {
			__pm3_assign_s(mps[i]->force, 0.0f);
		}

		// compute forces for particle p that are
		// originated within the mesh's structure
		m->update_forces();

		for (size_t p_idx = 0; p_idx < N; ++p_idx) {
			// ignore fixed particles
			if (mps[p_idx]->fixed) {
				continue;
			}

			// compute forces for particle p that are
			// originated by the force fields of the
			// simulation
			compute_forces(mps[p_idx]);

			// apply solver to predict next position and
			// velocity of the particle
			math::vec3 pred_pos, pred_vel;
			apply_solver(mps[p_idx], pred_pos, pred_vel);

			// check if there is any collision between
			// this mesh particle and a geometrical object

			/* The algorithm for updating a particle's position
			 * and velocity is the same as for a free particle.
			 * The only change here is the call to the appropriate
			 * function in the geometry class for a meshe's particle
			 * update.
			 */

			particles::from_mesh_to_free(*mps[p_idx], coll_pred);

			// has there been any collision?
			bool collision = false;

			// Check collision between the particle and
			// every fixed geometrical object in the scene.

			for (unsigned int g_idx = 0; g_idx < scene_fixed.size(); ++g_idx) {
				const geometry::geometry *g = scene_fixed[g_idx];

				// if the particle collides with some geometry
				// then the geometry is in charge of updating
				// this particle's position, velocity, ...

				if (g->intersec_segment(mps[p_idx]->cur_pos, pred_pos)) {
					collision = true;

					particles::from_mesh_to_free(*mps[p_idx], coll_pred);

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
				particles::from_free_to_mesh(coll_pred, *mps[p_idx]);
			}
			else {
				mps[p_idx]->save_position();
				__pm3_assign_v(mps[p_idx]->cur_pos, pred_pos);
				__pm3_assign_v(mps[p_idx]->cur_vel, pred_vel);
			}

			// clear the force so that in the next iteration
			// for the mesh, the forces can be computed
			__pm3_assign_s(mps[p_idx]->force, 0.0f);
		}
	}
}

} // -- namespace physim

