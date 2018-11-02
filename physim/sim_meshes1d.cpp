#include <physim/simulator.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/fields/gravitational_planet.hpp>
#include <physim/math/math.hpp>

namespace physim {

void simulator::simulate_meshes1d() {

	for (meshes::mesh1d *m1 : m1ds) {

		/* check mesh start time
		 *
		 * check lifetime
		 *
		 */

		/* update a meshe's particles */
		particles::mesh_particle **mps = m1->get_particles();
		particles::mesh_particle *p = mps[0];
		size_t N = m1->size();

		for (size_t i = 0; i < N; ++i, ++p) {
			// ignore fixed particles
			if (p->fixed) {
				continue;
			}

			// compute forces for particle p
			compute_forces(p);

			// apply solver to predict next position and
			// velocity of the particle
			math::vec3 pred_pos, pred_vel;
			apply_solver(p, pred_pos, pred_vel);

			// check if there is any collision between
			// this mesh particle and a geometrical object
			check_collision_update(pred_pos, pred_vel, p);
		}
	}
}

} // -- namespace physim

