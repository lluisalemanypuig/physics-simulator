#pragma once

// C inlcudes
#include <stddef.h>

// physim includes
#include <physim/math/vec3.hpp>

namespace physim {
namespace particles {

/**
 * @brief Class implementing a particle.
 *
 * A particle is a 0-dimensional object, subject to
 * several forces. It can also collide with other
 * objects in the scene (geometrical objects, see
 * namespace @ref physim::geom) but not with other
 * particles.
 *
 * When specified in the simulator, the moving particle
 * may create a variable electric field that acts on
 * the other particles.
 */
class mesh_particle {
	private:

		/**
		 * @brief Initialises all particle's attributes, most of them
		 * to null values.
		 *
		 * The attributes of the class take the following values:
		 * - @ref prev_pos : vec3(0,0,0)
		 * - @ref cur_velocity : vec3(0,0,0)
		 * - @ref force : vec3(0,0,0)
		 * - @ref mass : 1
		 * - @ref charge : 0
		 * - @ref fixed : false
		 * - @ref index : no value assigned, since it will be
		 * overwritten by the simulator.
		 *
		 * The current position (@ref cur_pos) is not initialised
		 * since it will be overwritten later in the methods that
		 * also call this one.
		 */
		void init();

	public:
		/// Previous position of the particle [m].
		math::vec3 prev_pos;
		/// Current position of the particle [m].
		math::vec3 cur_pos;
		/// Current velocity of the particle [m/s].
		math::vec3 cur_velocity;
		/// Force currently applied to the particle [N].
		math::vec3 force;

		/// Mass of the particle [Kg].
		float mass;
		/// Electrical charge of the particle [C].
		float charge;

		/**
		 * @brief Is this particle fixed?
		 *
		 * If the particle, it will be ignored by the solver, therefore
		 * not taken into account in the simulation (gravity nor any
		 * other force will have any effect on it).
		 */
		bool fixed;

		/**
		 * @brief Index of the particle.
		 *
		 * This index is automatically set when added to the
		 * simulator object. The collection of indexes determine
		 * the order in which particles have been added to it.
		 * The indexes start at 0.
		 *
		 * It can be used to initialise its attributes through
		 * the @ref init::initialiser class.
		 */
		size_t index;

	public:
		/// Default constructor.
		mesh_particle();
		/// Copy constructor.
		mesh_particle(const mesh_particle& p);
		/// Destructor.
		~mesh_particle();

		// MODIFIERS

		/**
		 * @brief Saves the current position in the particle's state.
		 *
		 * Copies @ref cur_pos into @ref prev_pos.
		 */
		void save_position();

};

} // -- namespace particles
} // -- namespace sim
