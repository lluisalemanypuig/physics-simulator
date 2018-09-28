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
		 * @brief Apply a time step to the simulation.
		 *
		 * Particles move according to time. Parameter
		 * @e dt indicates how much time has passed since
		 * the last time step.
		 * @param dt Floating point value strictly greater
		 * than 0.
		 */
		void apply_time_step(float dt);

		// GETTERS

		/// Get i-th particle.
		particle& get_particle(size_t i);
		/// Get i-th particle.
		const particle& get_particle(size_t i) const;
};

} // -- namespace sim
