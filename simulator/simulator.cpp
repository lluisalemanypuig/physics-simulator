#include <simulator/simulator.hpp>

namespace physim {

// PRIVATE

void simulator::init_particle(particle *p) {
	p->set_force(gravity);
	global_init->initialise_particle(p);
}

void simulator::update_particle(float dt, particle *p) {
	p->save_position();
	p->save_velocity();

	const vec3& force = p->get_force();
	const vec3& cur_vel = p->get_velocity();
	vec3 pre_pos = p->get_previous_position();
	vec3 cur_pos = p->get_position();
	vec3 next_pos;

	switch (solver) {
		case solver_type::EulerOrig:
			p->translate( cur_vel*dt );
			p->acceleterate( force*dt );
			break;

		case solver_type::EulerSemi:
			p->acceleterate( force*dt );
			p->translate( cur_vel*dt );
			break;

		case solver_type::Verlet:
			next_pos = cur_pos + (cur_pos - pre_pos) + force*dt*dt;
			p->set_position( next_pos );
			p->set_velocity( (next_pos - cur_pos)/dt );
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

	global_init = new initialiser();
}

simulator::~simulator() {
	clear_geometry();
	clear_particles();
	delete global_init;
}

// MODIFIERS

const particle *simulator::add_particle() {
	particle *p = new particle();
	p->set_index(ps.size());
	init_particle(p);
	ps.push_back(p);
	return p;
}

void simulator::add_particle(particle *p) {
	assert(p != nullptr);
	p->set_index(ps.size());
	ps.push_back(p);
}

void simulator::add_particles(size_t n) {
	for (size_t i = 0; i < n; ++i) {
		add_particle();
	}
}

void simulator::remove_particle(size_t i) {
	assert(i < ps.size());

	// in the position i:
	// 1. free the memory,
	// 2. put a valid pointer to another particle,
	// to the vector: shrink it to delete the
	// unused position at the end

	delete ps[i];
	size_t N = ps.size();
	std::swap(ps[i], ps[N - 1]);
	ps.pop_back();
}

void simulator::clear_particles() {
	for (particle *p : ps) {
		delete p;
	}
	ps.clear();
}

void simulator::add_geometry(geometry *g) {
	assert(g != nullptr);
	scene_fixed.push_back(g);
}

void simulator::remove_geometry(size_t i) {
	assert(i < scene_fixed.size());

	// in the position i:
	// 1. free the memory,
	// 2. put a valid pointer to another particle,
	// to the vector: shrink it to delete the
	// unused position at the end

	delete scene_fixed[i];
	size_t N = scene_fixed.size();
	std::swap(scene_fixed[i], scene_fixed[N - 1]);
	scene_fixed.pop_back();
}

void simulator::clear_geometry() {
	for (geometry *g : scene_fixed) {
		delete g;
	}
	scene_fixed.clear();
}

void simulator::clear_simulation() {
	clear_particles();
	clear_geometry();
	stime = 0.0f;
}

void simulator::reset_simulation() {
	stime = 0.0f;
	int i = 0;
	for (particle *p : ps) {
		if (not p->is_fixed()) {
			global_init->initialise_particle(p);
		}
		++i;
	}
}

void simulator::apply_time_step(float dt) {
	assert(dt > 0.0f);

	for (particle *p : ps) {
		// ignore fixed particles
		if (p->is_fixed()) {
			continue;
		}
		// Reset a particle when it dies.
		// Do not smiulate this particle
		// until the next step
		if (p->get_lifetime() <= 0.0f) {
			init_particle(p);
			continue;
		}
		// is this particle allowed to move?
		// if not, ignore it
		p->reduce_starttime(dt);
		if (p->get_starttime() > 0.0f) {
			continue;
		}

		// Particles age: reduce their lifetime.
		p->reduce_lifetime(dt);

		// apply solver to predict next position and
		// velocity of the particle
		update_particle(dt, p);

		/* The following two lines are equivalent to:
		 *
		 *	vec3 prev_pos = p->get_position()
		 *	update_particle(dt, p);
		 *	vec3 pred_pos = p->get_position();
		 */

		const vec3& prev_pos = p->get_previous_position();
		vec3 pred_pos = p->get_position();

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

		//cout << "----------------------" << endl;
		//cout << "At (" << prev_pos.x << "," << prev_pos.y << "," << prev_pos.z << ")" << endl;

		for (unsigned int i = 0; i < scene_fixed.size(); ++i) {
			const geometry *g = scene_fixed[i];

			// if the particle collides with some geometry
			// then the geometry is in charge of updating
			// this particle's position, velocity, ...

			//cout << "Detect collision of segment:" << endl;
			//cout << "    (" << prev_pos.x << "," << prev_pos.y << "," << prev_pos.z << ")" << endl;
			//cout << "    (" << pred_pos.x << "," << pred_pos.y << "," << pred_pos.z << ")" << endl;
			//cout << "against geometry:" << endl;
			//g->display();

			if (g->intersec_segment(prev_pos, pred_pos)) {
				//cout << "Collision!" << endl;

				collide = true;

				// recover original particle's state
				pred_particle = *p;

				// geometry updates the predicted particle
				g->update_upon_collision(&pred_particle);

				// keep track of the predicted particle's position
				pred_pos = pred_particle.get_position();
			}
			else {
				//cout << "No collision!" << endl;
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

void simulator::set_initialiser(const initialiser *f) {
	assert(f != nullptr);

	delete global_init;
	global_init = f->clone();
}

void simulator::set_solver(const solver_type& s) {
	solver = s;
}

// GETTERS

const vector<particle *>& simulator::get_particles() const {
	return ps;
}

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

size_t simulator::n_particles() const {
	return ps.size();
}

size_t simulator::n_geometry() const {
	return scene_fixed.size();
}

initialiser *simulator::get_initialiser() {
	return global_init;
}

const initialiser *simulator::get_initialiser() const {
	return global_init;
}

} // -- namespace physim
