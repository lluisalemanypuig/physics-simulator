#include <physim/simulator.hpp>

namespace physim {

// PRIVATE

void simulator::init_particle(particle *p) {

	//p->set_force(p->get_mass()*gravity);
	__pm_mul_v_s(p->get_force(), gravity, p->get_mass());

	global_init->initialise_particle(p);

	// Update the previous position so that Verlet
	// solver can use it correcly. The other solvers
	// do not need it, however.

	// p->set_previous_position(p->get_position() - p->get_velocity()*dt);

	__pm_mul_v_s(p->get_previous_position(), p->get_velocity(), dt);
	__pm_sub_v_v(p->get_previous_position(), p->get_position(), p->get_previous_position());
}

void simulator::apply_solver(const particle *p, math::vec3& pred_pos, math::vec3& pred_vel) {
	const float mass = p->get_mass();
	math::vec3 temp;

	switch (solver) {
		case solver_type::EulerOrig:
			/*
			pred_pos = p->get_position() + p->get_velocity()*dt;
			pred_vel = p->get_velocity() + (force/mass)*dt;
			*/

			__pm_mul_v_s(pred_pos, p->get_velocity(), dt);	// pred_pos <- velocity*dt
			__pm_add_acc_v(pred_pos, p->get_position());	// pred_pos <- position + pred_pos
															//			<- position + velocity*dt

			__pm_mul_v_s(pred_vel, p->get_force(), dt/mass);	// pred_vel <- force*(dt/mass)
			__pm_add_acc_v(pred_vel, p->get_velocity());	// pred_vel <- velocity + pred_vel
															//			<- velocity + force*(dt/mass)
			break;

		case solver_type::EulerSemi:
			/*
			pred_vel = p->get_velocity() + (force/mass)*dt;
			pred_pos = p->get_position() + pred_vel*dt;
			*/

			__pm_mul_v_s(pred_vel, p->get_force(), dt/mass);	// pred_vel <- force*(dt/mass)
			__pm_add_acc_v(pred_vel, p->get_velocity());	// pred_vel <- velocity + pred_vel
															//			<- velocity + force*(dt/mass)

			__pm_mul_v_s(pred_pos, pred_vel, dt);			// pred_pos <- velocity*dt
			__pm_add_acc_v(pred_pos, p->get_position());	// pred_pos <- position + pred_pos
															//			<- position + velocity*dt
			break;

		case solver_type::Verlet:
			/*
			pred_pos =
				p->get_position() +
				1.00000f*(p->get_position() - p->get_previous_position()) +
				(force/mass)*dt*dt;
			pred_vel = (pred_pos - p->get_position())/dt;
			*/

			// temp <- force*((dt*dt)/mass)
			__pm_mul_v_s(temp, p->get_force(), (dt*dt)/mass);
			// temp <- cur_pos + force*((dt*dt)/mass)
			__pm_add_v_v(temp, p->get_position(), temp);
			// temp <- cur_pos + force*((dt*dt)/mass) - prev_pos
			//      <- cur_pos - prev_pos + force*((dt*dt)/mass)
			__pm_sub_v_v(temp, temp, p->get_previous_position());
			// pred_pos <- cur_position + temp
			//			<- cur_position + 1.0f*(cur_pos - prev_pos)
			//			   + force*((dt*dt)/mass)
			__pm_add_v_v(pred_pos, p->get_position(), temp);

			__pm_sub_v_v(pred_vel, pred_pos, p->get_position());
			__pm_div_v_s(pred_vel, pred_vel, dt);
			break;

		default:
			cerr << "Warning: solver not implemented" << endl;
	}
}

// PUBLIC

simulator::simulator(const solver_type& s, float t) {
	__pm_assign_c(gravity, 0.0f, -9.81f, 0.0f);
	stime = 0.0f;
	dt = t;
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
			init_particle(p);
		}
		++i;
	}
}

void simulator::apply_time_step() {

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
		particle coll_pred;

		// has there been any collision?
		bool collision = false;

		// Check collision between the particle and
		// every fixed geometrical object in the scene.

		for (unsigned int i = 0; i < scene_fixed.size(); ++i) {
			const geometry *g = scene_fixed[i];

			// if the particle collides with some geometry
			// then the geometry is in charge of updating
			// this particle's position, velocity, ...

			if (g->intersec_segment(p->get_position(), pred_pos)) {
				collision = true;

				coll_pred = *p;

				// the geometry updates the predicted particle
				g->update_upon_collision(pred_pos, pred_vel, &coll_pred);

				if (solver == solver_type::Verlet) {
					// this solver needs a correct position
					// for the 'previous' position of the
					// particle after a collision with geometry

					/*coll_pred.set_previous_position(
						coll_pred.get_position() - coll_pred.get_velocity()*dt
					);*/

					math::vec3& cp_prev_pos = coll_pred.get_previous_position();
					__pm_mul_v_s(cp_prev_pos, coll_pred.get_velocity(), dt);
					__pm_sub_v_v(cp_prev_pos, coll_pred.get_position(), cp_prev_pos);
				}

				// keep track of the predicted particle's position
				/*pred_pos = coll_pred.get_position();
				pred_vel = coll_pred.get_velocity();*/
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

void simulator::set_gravity(const math::vec3& g) {
	//gravity = g;
	__pm_assign_v(gravity, g);
}

void simulator::set_time_step(float t) {
	dt = t;
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

const math::vec3& simulator::get_gravity() const {
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
