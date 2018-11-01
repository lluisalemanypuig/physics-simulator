#include <physim/simulator.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/fields/gravitational_planet.hpp>
#include <physim/math/math.hpp>

namespace physim {

// PRIVATE

void simulator::init_particle(free_particle *p) {
	global_init->initialise_particle(p);

	// Update the previous position so that Verlet
	// solver can use it correcly. The other solvers
	// do not need it, however.

	// prev_pos <- pos - vel*dt
	__pm_sub_v_vs(p->get_previous_position(), p->get_position(), p->get_velocity(), dt);
}

void simulator::apply_solver(const free_particle *p, math::vec3& pred_pos, math::vec3& pred_vel) {
	const float mass = p->get_mass();

	switch (solver) {
		case solver_type::EulerOrig:
			// pred_pos <- pos + vel*dt
			__pm_add_v_vs(pred_pos, p->get_position(), p->get_velocity(), dt);
			// pred_vel <- vel + force*dt/mass
			__pm_add_v_vs(pred_vel, p->get_velocity(), p->get_force(), dt/mass);
			break;

		case solver_type::EulerSemi:
			// pred_vel <- vel + force*dt/mass
			__pm_add_v_vs(pred_vel, p->get_velocity(), p->get_force(), dt/mass);
			// pred_pos <- pos + pred_vel*dt
			__pm_add_v_vs(pred_pos, p->get_position(), pred_vel, dt);
			break;

		case solver_type::Verlet:
			// pred_pos <- pos + 1.0f*(pos - prev_pos) + force*dt*dt/m
			__pm_sub_v_v(pred_pos, p->get_position(), p->get_previous_position());
			__pm_add_vs_vs_v(pred_pos, pred_pos,1.0f, p->get_force(),(dt*dt)*(1.0f/mass), p->get_position());

			// pred_vel <- (pred_pos - pos)/dt
			__pm_sub_v_v_div_s(pred_vel, pred_pos, p->get_position(), dt);
			break;

		default:
			std::cerr << "Warning: solver not implemented" << std::endl;
	}
}

void simulator::compute_forces(free_particle *p) {
	// clear the current force
	__pm_assign_s(p->get_force(), 0.0f);

	// compute the force every force field
	// makes on every particle
	math::vec3 F;
	for (fields::field *f : force_fields) {
		f->compute_force(p, F);
		__pm_add_acc_v(p->get_force(), F);
	}
}

// PUBLIC

simulator::simulator(const solver_type& s, float t) {
	__pm_assign_c(gravity, 0.0f, -9.81f, 0.0f);
	stime = 0.0f;
	dt = t;
	solver = s;
	global_init = new init::initialiser();
}

simulator::~simulator() {
	clear_geometry();
	clear_particles();
	delete global_init;
}

// MODIFIERS

const free_particle *simulator::add_particle() {
	free_particle *p = new free_particle();
	p->set_index(ps.size());
	init_particle(p);
	ps.push_back(p);
	return p;
}

void simulator::add_particle(free_particle *p) {
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
	for (free_particle *p : ps) {
		delete p;
	}
	ps.clear();
}

/* GEOMETRY */

void simulator::add_geometry(geom::geometry *g) {
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
	for (geom::geometry *g : scene_fixed) {
		delete g;
	}
	scene_fixed.clear();
}

/* FIELDS */

void simulator::add_field(fields::field *f) {
	assert(f != nullptr);
	force_fields.push_back(f);
}

void simulator::remove_field(size_t i) {
	assert(i < force_fields.size());

	// in the position i:
	// 1. free the memory,
	// 2. put a valid pointer to another particle,
	// to the vector: shrink it to delete the
	// unused position at the end

	delete force_fields[i];
	size_t N = force_fields.size();
	std::swap(force_fields[i], force_fields[N - 1]);
	force_fields.pop_back();
}

void simulator::clear_fields() {
	for (fields::field *f : force_fields) {
		delete f;
	}
	force_fields.clear();
}

/* SIMULATION */

void simulator::clear_simulation() {
	clear_particles();
	clear_geometry();
	stime = 0.0f;
}

void simulator::reset_simulation() {
	stime = 0.0f;
	int i = 0;
	for (free_particle *p : ps) {
		if (not p->is_fixed()) {
			init_particle(p);
		}
		++i;
	}
}

void simulator::apply_time_step() {

	for (free_particle *p : ps) {
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

		// compute forces for particle p
		compute_forces(p);

		// Particles age: reduce their lifetime.
		p->reduce_lifetime(dt);

		// apply solver to predict next position and
		// velocity of the particle
		math::vec3 pred_pos, pred_vel;
		apply_solver(p, pred_pos, pred_vel);

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

		// collision prediction
		// copy the particle at its current state and use it
		// to predict the update upon collision with geometry
		free_particle coll_pred;

		// has there been any collision?
		bool collision = false;

		// Check collision between the particle and
		// every fixed geometrical object in the scene.

		for (unsigned int i = 0; i < scene_fixed.size(); ++i) {
			const geom::geometry *g = scene_fixed[i];

			// if the particle collides with some geometry
			// then the geometry is in charge of updating
			// this particle's position, velocity, ...

			if (g->intersec_segment(p->get_position(), pred_pos)) {
				collision = true;

				coll_pred = *p;

				// the geometry updates the predicted particle
				g->update_particle(pred_pos, pred_vel, &coll_pred);

				if (solver == solver_type::Verlet) {
					// this solver needs a correct position
					// for the 'previous' position of the
					// particle after a collision with geometry

					math::vec3& cp_prev_pos = coll_pred.get_previous_position();
					__pm_sub_v_vs(cp_prev_pos, coll_pred.get_position(), coll_pred.get_velocity(), dt);
				}

				// keep track of the predicted particle's position
				__pm_assign_v(pred_pos, coll_pred.get_position());
				__pm_assign_v(pred_vel, coll_pred.get_velocity());
			}
		}

		// give the particle the proper final state
		if (collision) {
			*p = coll_pred;
		}
		else {
			p->save_position();
			p->set_position(pred_pos);
			p->set_velocity(pred_vel);
		}
	}

	stime += dt;
}

// SETTERS

void simulator::add_gravity_acceleration(const math::vec3& g) {
	fields::gravitational_planet *f = new fields::gravitational_planet(g);
	force_fields.push_back(f);
}

void simulator::set_time_step(float t) {
	dt = t;
}

void simulator::set_initialiser(const init::initialiser *f) {
	assert(f != nullptr);

	delete global_init;
	global_init = f->clone();
}

void simulator::set_solver(const solver_type& s) {
	solver = s;
}

// GETTERS

const std::vector<free_particle *>& simulator::get_particles() const {
	return ps;
}

const free_particle& simulator::get_particle(size_t i) const {
	return *ps[i];
}

const std::vector<geom::geometry *>& simulator::get_fixed_objects() const {
	return scene_fixed;
}

float simulator::get_current_time() const {
	return stime;
}

const math::vec3& simulator::get_gravity() const {
	return gravity;
}

size_t simulator::n_particles() const {
	return ps.size();
}

size_t simulator::n_geometry() const {
	return scene_fixed.size();
}

init::initialiser *simulator::get_initialiser() {
	return global_init;
}

const init::initialiser *simulator::get_initialiser() const {
	return global_init;
}

} // -- namespace physim
