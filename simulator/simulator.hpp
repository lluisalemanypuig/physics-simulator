#pragma once

// C includes
#include <assert.h>

// C++ includes
#include <functional>
#include <iostream>
#include <cstdint>
#include <vector>
using namespace std;

// Custom includes
#include <simulator/geometry/geometry.hpp>
#include <simulator/geometry/triangle.hpp>
#include <simulator/geometry/sphere.hpp>
#include <simulator/geometry/plane.hpp>
#include <simulator/particle.hpp>

namespace sim {
using namespace geom;

/**
 * @brief The different types of solvers.
 *
 * - EulerOrig: implements the usual version of the Euler solver.
 * Position and velocity are updated with the usual formulas, and
 * in this order.
 \verbatim
 xc := current position
 new position = xc + time step * current velocity
 new volcity = xc + time step * force / particle mass
 \endverbatim
 *
 * - EulerSemi: implements a slightly different version from the
 * @e EulerOrig solver. Although it uses the same formulas,
 * the velocity is updated first, and then the position is updated.
 *
 * - Verlet: this solver does not use the particle's velocity to update
 * its position. However, the velocity is still updated.
 \verbatim
 xc := current position
 xp := previous position
 new position = xc + k*(xc - xp) + force * time step * force / particle mass
 new velocity = (xn - xc)/time step
 \endverbatim
 */
enum class solver_type : int8_t {
	none = -1,
	EulerOrig, EulerSemi, Verlet
};

/**
 * @brief Simulator class.
 *
 * This class contains the algorihm used to simulate the
 * movement of particles and systems of particles (like
 * springs systems).
 *
 * Use method @ref apply_time_step(float) to make the
 * simulation run.
 */
class simulator {
	private:
		/**
		 * @brief The collection of objects whose position
		 * does not depend on the interaction with other objects.
		 */
		vector<geometry *> scene_fixed;
		/// The set of particles in the simulation
		vector<particle *> ps;

		/// Gravity of the simulation.
		vec3 gravity;
		/// Current time of the simulation.
		float stime;
		/// Solver used to update each particle's position.
		solver_type solver;

		/**
		 * @brief Initialiser applied to all added particles.
		 *
		 * Whenever @ref add_particle() or @ref add_particles(size_t)
		 * are called, the attributes of the particles are initialised
		 * using @ref global_init.
		 *
		 * This initialisation is also done whenever the lifetime of a
		 * particle goes below 0. Note that, in this case, some attributes
		 * like the velocity and position may not be zero, as a result
		 * of running the simulation.
		 *
		 * The default behaviour of the function is not to do anything,
		 * that is, the attribtues of the particle are not modified
		 * at all.
		 *
		 * It is important to mention that the force applied to a particle
		 * is always set to be equal to the simulator's gravity (see @ref gravity).
		 * However, it is set before calling the initialiser function.
		 * Therefore, it is recommended to use the method @ref particle::add_force
		 * if it is to be modified.
		 */
		function<void (particle *p)> global_init;

	private:

		/**
		 * @brief Initialises a particle using @ref global_init.
		 *
		 * First, it sets the force applied to the particle to be
		 * the simulator's gravity (see @ref gravity). Then,
		 * calls @ref global_init to initialise the rest of its
		 * attributes.
		 * @param p The particle to be initialsed.
		 */
		void init_particle(particle *p);

		/**
		 * @brief Update a particle's position.
		 * @param p Particle to be updated.
		 * @param cur_pos Current position of the particle.
		 * @param dt Time step applied.
		 */
		void update_particle(const vec3& cur_pos, float dt, particle *p);
		/**
		 * @brief Update a particle collision in a collision with a plane.
		 *
		 * Assumig that particle @e p collided with the plane defined by
		 * the normal vector @e n and constant term @e d, update its
		 * position accordingly. This method is defined specially for
		 * collision with planes.
		 * @param n Normal of the plane the particle collided with.
		 * @param d Constant, independent term of the plane's equation.
		 * @param next_pos The position of the particle predicted by the solver.
		 * @param next_vel The velocity of the particle predicted by the solver.
		 * @param p The particle to be updated.
		 */
		void update_collision_plane
		(
			const vec3& n, float d,
			const vec3& next_pos, const vec3& next_vel,
			particle *p
		);
		/**
		 * @brief Update a particle collision in a collision with geometry.
		 *
		 * Assumig that particle @e p collided with geometry @e g, update its
		 * position accordingly.
		 * @param g The geometry the particle collided with.
		 * @param prev The position of the particle before update.
		 * @param inter Intersection point between the segment [@e prev, @e cur] and the geometry.
		 * @param next The position of the particle after update.
		 * @param dt The time step used.
		 * @param p The particle to be updated.
		 */
		void update_upon_collision
		(
			const geometry *g,
			const vec3& prev, const vec3& inter, const vec3& next,
			float dt, particle *p
		);

	public:
		/// Default constructor
		simulator(const solver_type& s = solver_type::EulerOrig);
		/// Destructor
		~simulator();

		// MODIFIERS

		/// Adds a particle to the simulation.
		const particle *add_particle();
		/**
		 * @brief Adds the particle passed as parameter to the simulation.
		 *
		 * The initialser function (see @ref global_init) is not called.
		 * @param p A non-null pointer to the object.
		 */
		void add_particle(particle *p);
		/// Adds @e n particles to the simulation.
		void add_particles(size_t n);

		/**
		 * @brief Adds a plane to the scene.
		 *
		 * The geometrical object is added to @ref scene_fixed.
		 * @param p A non-null pointer to the object.
		 */
		void add_plane(plane *p);
		/**
		 * @brief Adds a triangle to the scene.
		 *
		 * The geometrical object is added to @ref scene_fixed.
		 * @param t A non-null pointer to the object.
		 */
		void add_triangle(triangle *t);
		/**
		 * @brief Adds a sphere to the scene.
		 *
		 * The geometrical object is added to @ref scene_fixed.
		 * @param s A non-null pointer to the object.
		 */
		void add_sphere(sphere *s);

		/**
		 * @brief Resets the simulation to its original state.
		 *
		 * Sets to 0 the time of the simulation (see @ref stime).
		 */
		void reset_simulation();

		/**
		 * @brief Apply a time step to the simulation.
		 *
		 * Particles move according to time. Parameter @e dt indicates
		 * how much time has passed since the last time step.
		 *
		 * Whenever a particle's lifetime has reached 0, method
		 * @ref init_particle(particle*) is called.
		 *
		 * @param dt Strictly positive value representing how much the
		 * time must be incremented.
		 */
		void apply_time_step(float dt);

		// SETTERS

		/**
		 * @brief Sets the gravity vector.
		 *
		 * Gravity is always applied to all particles, except those
		 * that are fixed.
		 */
		void set_gravity(const vec3& g);

		/**
		 * @brief Sets the particle initialiser function.
		 * @param f This function need not return any value. The
		 * pointer passed as parameter is always guaranteed to
		 * be non-null. This function will be called the first time
		 * the lifetime of the particle has reached a value equal to
		 * or smaller than 0..
		 */
		void set_initialiser(const function<void (particle *p)>& f);

		// GETTERS

		/// Return constant reference to i-th particle.
		const particle& get_particle(size_t i) const;
		/// Returns all particles.
		const vector<particle *>& get_all_particles() const;
		/// Returns all fixed objects of the scene.
		const vector<geometry *>& get_fixed_objects() const;
		/// Returns the current simulation time.
		float get_current_time() const;
		/// Returns the gravity of the scene.
		const vec3& get_gravity() const;
};

} // -- namespace sim
