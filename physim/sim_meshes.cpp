#include <physim/simulator.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/fields/gravitational_planet.hpp>
#include <physim/particles/conversions.hpp>
#include <physim/math/private/math3/base.hpp>
#include <physim/sim_solver.cpp>

namespace physim {
using namespace particles;
using namespace meshes;
using namespace math;

void simulator::_simulate_meshes() {

	// collision prediction:
	// copy the particle at its current state and use it
	// to predict the update upon collision with geometry.
	// Although it is a free particle, the attributes
	// of the mesh particle will be copied into this one.
	free_particle current;
	free_particle coll_pred;

	for (mesh *m : ms) {

		/* check mesh start time and lifetime
		 * of mesh
		 */

		// some of the meshe's attributes are needed
		// in the collision prediction particle for...
		// collision prediction
		coll_pred.friction = m->get_friction();
		coll_pred.bouncing = m->get_bouncing();

		/* update a meshe's particles */
		mesh_particle **mps = m->get_particles();
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
			vec3 pred_pos, pred_vel;
			apply_solver(mps[p_idx], pred_pos, pred_vel);

			// check if there is any collision between
			// this mesh particle and a geometrical object

			/* The algorithm for updating a particle's position
			 * and velocity is the same as for a free particle.
			 * The only change here is the call to the appropriate
			 * function in the geometry class for a meshe's particle
			 * update.
			 */

			from_mesh_to_free(*mps[p_idx], current);
			from_mesh_to_free(*mps[p_idx], coll_pred);

			bool collision =
			find_update_geomcoll_free(&current, pred_pos, pred_vel, coll_pred);

			if (part_part_colls_activated()) {
				bool r = find_update_partcoll_free
				(&current, pred_pos, pred_vel, coll_pred);

				collision = collision or r;
			}

			// give the particle the proper final state
			if (collision) {
				from_free_to_mesh(coll_pred, *mps[p_idx]);
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

