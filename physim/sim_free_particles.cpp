#include <physim/simulator.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/fields/gravitational_planet.hpp>
#include <physim/math/math.hpp>
#include <physim/sim_solver.cpp>

namespace physim {

void simulator::simulate_free_particles() {

	for (particles::free_particle *p : ps) {
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
		check_collision_update(pred_pos, pred_vel, p);
	}

}

} // -- namespace physim
