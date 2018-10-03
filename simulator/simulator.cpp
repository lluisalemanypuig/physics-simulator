#include <simulator/simulator.hpp>

namespace sim {

// PRIVATE

void simulator::init_particle(particle *p) {
	p->set_force(gravity);
	global_init(p);
}

void simulator::update_particle(const vec3& cur_pos, float dt, particle *p) {

	switch (solver) {
		case solver_type::EulerOrig:
			p->set_prev_position( cur_pos );
			p->translate( p->get_velocity()*dt );
			p->acceleterate( p->get_force()*dt );
			break;

		case solver_type::EulerSemi:
			p->set_prev_position( cur_pos );
			p->acceleterate( p->get_force()*dt );
			p->translate( p->get_velocity()*dt );
			break;

		case solver_type::Verlet:
			break;
		default:
			cerr << "Error: no solver assigned" << endl;
	}
}

// PUBLIC

simulator::simulator(const solver_type& s) {
	gravity = vec3(0.0f, -9.81f, 0.0f);
	stime = 0.0f;
	solver = s;
	global_init = [](particle *) { };
}

simulator::~simulator() {
	for (geometry *g : scene_fixed) {
		if (g != nullptr) {
			delete g;
		}
	}
	for (particle *p : ps) {
		if (p != nullptr) {
			delete p;
		}
	}
}

// MODIFIERS

const particle *simulator::add_particle() {
	particle *p = new particle();
	init_particle(p);
	ps.push_back(p);
	return p;
}

void simulator::add_particle(particle *p) {
	assert(p != nullptr);
	ps.push_back(p);
}

void simulator::add_particles(size_t n) {
	for (size_t i = 0; i < n; ++i) {
		add_particle();
	}
}

void simulator::add_geometry(geometry *g) {
	assert(g != nullptr);
	scene_fixed.push_back(g);
}

void simulator::reset_simulation() {
	stime = 0.0f;
}

void simulator::apply_time_step(float dt) {
	assert(dt > 0.0f);

	for (particle *p : ps) {
		// ignore fixed particles
		if (p->is_fixed()) {
			continue;
		}
		// reset a particle whenever it dies
		if (p->get_lifetime() < 0.0f) {
			init_particle(p);
		}
		// particles age, like we all do :(
		p->reduce_lifetime(dt);

		// apply solver to predict next position and
		// velocity of the particle
		const vec3& prev_pos = p->get_current_position();
		update_particle(prev_pos, dt, p);
		vec3 pred_pos = p->get_current_position();

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

		// copy the particle at its current state
		// and use it to predict the update
		particle pred_particle;

		// Check collision between the particle and
		// every fixed geometrical object in the scene.
		// Set boolean to true if any collision took place.
		bool collide = false;

		for (unsigned int i = 0; i < scene_fixed.size(); ++i) {
			const geometry *g = scene_fixed[i];

			// if the particle collides with some geometry
			// then the geometry is in charge of updating
			// this particle's position, velocity, ...

			if (g->intersec_segment(prev_pos, pred_pos)) {
				collide = true;
				pred_particle = *p;

				// geometry updates the predicted particle
				g->update_upon_collision(&pred_particle);

				// keep track of the predicted particle's position
				pred_pos = pred_particle.get_current_position();
			}
		}

		// give the particle the proper final state
		if (collide) {
			*p = pred_particle;
		}
	}

	stime += dt;
}

// SETTERS

void simulator::set_gravity(const vec3& g) {
	gravity = g;
}

void simulator::set_initialiser(const function<void (particle *p)>& f) {
	global_init = f;
}

// GETTERS

const particle& simulator::get_particle(size_t i) const {
	return *ps[i];
}

const vector<geometry *>& simulator::get_fixed_objects() const {
	return scene_fixed;
}

float simulator::get_current_time() const {
	return stime;
}

const vec3& simulator::get_gravity() const {
	return gravity;
}

} // -- namespace sim
