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

void simulator::update_collision_plane
(
	const vec3& n, float d,
	const vec3& next_pos, const vec3& next_vel,
	particle *p
)
{
	vec3 Wn = (glm::dot(next_pos, n) + d)*n;
	float bounce = p->get_bouncing();
	p->set_position( next_pos - (1 + bounce)*Wn );

	float nv_dot = glm::dot(n, next_vel);
	p->set_velocity( next_vel - (1 + bounce)*(nv_dot*n) );
}

void simulator::update_upon_collision
(
	const geometry *g,
	const vec3& prev, const vec3& inter, const vec3& next,
	float dt, particle *p
)
{

	if (g->get_geom_type() == geom_type::Plane) {
		// nothing, already handled
	}

}

// PUBLIC

simulator::simulator(const solver_type& s) {
	gravity = vec3(0.0f, -9.81f, 0.0f);
	stime = 0.0f;
	solver = s;
	global_init = [](particle *p) { };
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

void simulator::add_plane(plane *p) {
	assert(p != nullptr);
	scene_fixed.push_back(p);
}

void simulator::add_triangle(triangle *t) {
	assert(t != nullptr);
	scene_fixed.push_back(t);
}

void simulator::add_sphere(sphere *s) {
	assert(s != nullptr);
	scene_fixed.push_back(s);
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
		vec3 next_vel = p->get_velocity();

		// intersection point between the particle and
		// the geometry
		vec3 inter(0.0f, 0.0f, 0.0f);

		// check collision between the particle and
		// every fixed geometrical object in the scene
		for (geometry *g : scene_fixed) {

			// for planes, collision is simpler to check
			if (g->get_geom_type() == geom_type::Plane) {

				const plane *pl = static_cast<const plane *>(g);
				if (pl->intersec_segment(prev_pos, next_pos)) {
					update_collision_plane
					(pl->get_normal(), pl->get_constant(), next_pos, next_vel, p);
				}
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
