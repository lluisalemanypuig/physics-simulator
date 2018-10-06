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

namespace physim {
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
 * Use method @ref apply_time_step to run the simulation
 * step by step.
 *
 * Use method @ref add_geometry to add fixed geometrical
 * objects (spheres, triangles, planes) with which the
 * particles may collide with.
 *
 * Function @ref reset_simulation resets all particle to
 * their original state using the initialiser function
 * (see @ref global_init).
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
		 * It is important to mention that the force applied on a particle
		 * is always set to be equal to the simulator's gravity (see @ref
		 * gravity). However, it is set before calling the initialiser
		 * function. Therefore, it is recommended to use the method @ref
		 * particle::add_force if it is to be modified.
		 *
		 * It is recommended to set the particle's 'previous' state
		 * to null values.
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
		 * @param dt Time step applied.
		 */
		void update_particle(float dt, particle *p);

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
		 *
		 * The caller should not free the object, since the simulator
		 * will take care of that.
		 * @param p A non-null pointer to the object.
		 */
		void add_particle(particle *p);
		/// Adds @e n particles to the simulation.
		void add_particles(size_t n);

		/**
		 * @brief Removes the @e i-th particle.
		 *
		 * Frees the memory occupied by the particle in the @e i-th
		 * position. Therefore, any pointer to that particle becomes
		 * invalid.
		 * @param i The index of the particle in [0, number of particles).
		 */
		void remove_particle(size_t i);
		/**
		 * @brief Deletes all particles in this simulator.
		 *
		 * Deletes all the objects in @ref ps and clears the
		 * container.
		 */
		void clear_particles();

		/**
		 * @brief Adds a geomtrical object to the scene.
		 *
		 * The geometrical object is added to @ref scene_fixed.
		 *
		 * The caller should not free the object, since the simulator
		 * will take care of that.
		 * @param g A non-null pointer to the object.
		 */
		void add_geometry(geometry *g);

		/**
		 * @brief Removes the @e i-th fixed geometrical object.
		 *
		 * Frees the memory occupied by the geometrical object in the @e i-th
		 * position. Therefore, any pointer to that object becomes
		 * invalid.
		 * @param i The index of the object in [0, number of geometrical objects).
		 */
		void remove_geometry(size_t i);
		/**
		 * @brief Deletes all geometry in this simulator.
		 *
		 * Deletes all the objects in @ref scene_fixed and clears
		 * the container.
		 */
		void clear_geometry();
		/**
		 * @brief Clears the simulation to an empty state;
		 *
		 * Deletes all particles, geometry and resets the simulation time.
		 * 
		 * That is, calls @ref clear_particles, @ref clear_geometry,
		 * and sets @ref stime to 0.
		 */
		void clear_simulation();

		/**
		 * @brief Resets the simulation to its initial state.
		 *
		 * Resets the simulation time (see @ref stime) to 0 and
		 * initialises all particles with the initialiser function
		 * @ref global_init.
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
		 * or smaller than 0.
		 */
		void set_initialiser(const function<void (particle *p)>& f);

		// GETTERS

		/// Return constant reference to i-th particle.
		const particle& get_particle(size_t i) const;
		/// Returns all fixed objects of the scene.
		const vector<geometry *>& get_fixed_objects() const;
		/// Returns the current simulation time.
		float get_current_time() const;
		/// Returns the gravity of the scene.
		const vec3& get_gravity() const;
		/// Returns the number of particles.
		size_t n_particles() const;
		/// Returns the number of fixed geometrical objects.
		size_t n_geometry() const;
};

} // -- namespace sim
