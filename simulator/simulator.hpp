#pragma once

// C includes
#include <assert.h>

// C++ includes
#include <vector>
using namespace std;

// Custom includes
#include <simulator/geometry/geometry.hpp>
#include <simulator/geometry/triangle.hpp>
#include <simulator/geometry/sphere.hpp>
#include <simulator/geometry/plane.hpp>
#include <simulator/particle.hpp>

namespace sim {

enum class solver : std::int8_t {
	none = -1,
	EulerOrig, EulerSemi, Verlet
};

/**
 * @brief Simulator class.
 *
 * This class contains the algorihm used to simulate the
 * movement of particles and systems of particles (like
 * springs systems).
 */
class simulator {
	private:
		/**
		 * @brief The collection of objects whose position
		 * does not depend on the interaction with other objects.
		 */
		vector<geom::geometry *> scene_fixed;

		/// The set of particles in the simulation
		vector<particle *> ps;


		/// Gravity of the simulation.
		vec3 gravity;
		/// Current time of the simulation.
		float stime;

	public:
		/// Default constructor
		simulator();
		/// Destructor
		~simulator();

		// MODIFIERS

		/// Adds a particle to the simulation.
		void add_particle();
		/**
		 * @brief Adds the particle passed as parameter to the simulation.
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
		void add_plane(geom::plane *p);
		/**
		 * @brief Adds a triangle to the scene.
		 *
		 * The geometrical object is added to @ref scene_fixed.
		 * @param t A non-null pointer to the object.
		 */
		void add_triangle(geom::triangle *t);
		/**
		 * @brief Adds a sphere to the scene.
		 *
		 * The geometrical object is added to @ref scene_fixed.
		 * @param s A non-null pointer to the object.
		 */
		void add_sphere(geom::sphere *s);

		/**
		 * @brief Resets the simulation to its original state.
		 *
		 * Sets to 0 the time of the simulation (see @ref stime).
		 */
		void reset_simulation();

		/**
		 * @brief Apply a time step to the simulation.
		 *
		 * Particles move according to time. Parameter
		 * @e dt indicates how much time has passed since
		 * the last time step.
		 * @param dt Strictly positive value.
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

		// GETTERS

		/// Get i-th particle.
		const particle& get_particle(size_t i) const;
		/// Get all particles.
		const vector<particle *>& get_all_particles() const;
		/// Get all fixed objects of the scene.
		const vector<geom::geometry *>& get_fixed_objects() const;
		/// Get current time.
		float get_current_time() const;
};

} // -- namespace sim
