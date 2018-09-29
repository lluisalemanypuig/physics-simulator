#include <simulator/simulator.hpp>

namespace sim {

// PRIVATE

void simulator::init_particle(particle *p) {
	cout << "Initialise particle!" << endl;
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
		vec3 prev_pos = p->get_current_position();
		update_particle(prev_pos, dt, p);
		vec3 next_pos = p->get_current_position();

		// check collision between the particle and
		// every fixed geometrical object in the scene
		for (geometry *g : scene_fixed) {

			// if the particle collides with some geometry
			// then the geometry is in charge of updating
			// this particle's position, velocity, ...
			if (g->intersec_segment(prev_pos, next_pos)) {
				g->update_upon_collision(p);
			}
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

const vector<particle *>& simulator::get_all_particles() const {
	return ps;
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
