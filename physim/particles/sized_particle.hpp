#pragma once

// physim includes
#include <physim/particles/free_particle.hpp>

namespace physim {
namespace particles {

/**
 * @brief Class implementing a sized particle.
 *
 * A free particle is a 0-dimensional object, subject
 * to several forces but not to any other particle's
 * direct interaction through them. It can collide with
 * other objects in the scene (geometrical objects, see
 * namespace @ref physim::geom) and also with other
 * paricles.
 *
 * A sized particle is, then, a particle with some size
 * (the radius, see @ref R) along the three axis. The loci
 * of point defined by the sphere centered at the current
 * position of the particle (see @ref free_particle::cur_pos)
 * and radius @e R are part of the particle. Notice,
 * however, that this is not a sphere.
 *
 * All this class' attributes are public.
 */
class sized_particle : public free_particle {
	public:
		/// Radius of the particle [m].
		float R;

	public:
		/// Default constructor.
		sized_particle();
		/// Copy constructor.
		sized_particle(const sized_particle& p);
		/// Destructor.
		~sized_particle();

		/**
		 * @brief Initialises all particle's attributes, most of them
		 * to null values.
		 *
		 * The attributes of the class take the following values:
		 * - @ref prev_pos : vec3(0,0,0)
		 * - @ref cur_vel : vec3(0,0,0)
		 * - @ref force : vec3(0,0,0)
		 * - @ref mass : 1
		 * - @ref bouncing : 0.8
		 * - @ref friction : 0.2
		 * - @ref charge : 0
		 * - @ref lifetime : 10
		 * - @ref starttime : 0
		 * - @ref fixed : false
		 * - @ref R : 1.0
		 *
		 * The current position (@ref cur_pos) is not initialised
		 * since it will be overwritten later in the methods that
		 * also call this one. The other attributes, not mentioned
		 * in the list above, are not modified either for similar
		 * reasons.
		 */
		void init();

		// GETTERS

		/// Returns true.
		bool is_sized() const;
};

} // -- namespace particles
} // -- namespace physim
