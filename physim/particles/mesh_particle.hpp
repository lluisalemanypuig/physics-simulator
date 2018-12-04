#pragma once

// C inlcudes
#include <stddef.h>

// physim includes
#include <physim/particles/particle_types.hpp>
#include <physim/particles/base_particle.hpp>
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
class mesh_particle : public base_particle {
	private:
		/**
		 * @brief Initialises this class's attributes.
		 *
		 * The attributes of the class take the following values:
		 * - @ref charge : 0
		 * - @ref fixed : false
		 */
		void partial_init();

	public:
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

	public:
		/// Default constructor.
		mesh_particle();
		/// Copy constructor.
		mesh_particle(const mesh_particle& p);
		/// Destructor.
		~mesh_particle();

		// MODIFIERS

		/**
		 * @brief Initialises all particle's attributes, most of them
		 * to null values.
		 *
		 * The attributes of the class take the following values:
		 * - @ref prev_pos : vec3(0,0,0)
		 * - @ref cur_vel : vec3(0,0,0)
		 * - @ref force : vec3(0,0,0)
		 * - @ref mass : 1
		 * - @ref index : no value assigned, since it will be
		 * overwritten by the simulator.
		 * - @ref charge : 0
		 * - @ref fixed : false
		 */
		virtual void init();

		virtual particle_type get_particle_type() const;

};

} // -- namespace particles
} // -- namespace sim
