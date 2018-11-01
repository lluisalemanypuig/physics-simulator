#pragma once

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/fields/magnetic.hpp>

namespace physim {
namespace fields {

/**
 * @brief Implementation of a magnetic B-field.
 *
 * The force that acts on a particle is calculated
 * as:
 *
 * \f$ \vec{F} = (q \cdot v_p) \times \vec{B}\f$,
 * where \f$q\f$ is the charge of the particle
 * (see @ref particle::charge), \f$B\f$ is the magnetic
 * field vector (see @ref magnetic_field::B) and
 * \f$v_p\f$ is the particle's punctual velocity
 * (see @ref particle::cur_vel).
 */
class magnetic_B : public magnetic {
	protected:

	public:
		/// Default constructor
		magnetic_B();
		/// Constructor with position and magnetic field vector.
		magnetic_B(const math::vec3& pos, const math::vec3& b);
		/// Copy constructor.
		magnetic_B(const magnetic_B& f);
		/// Destructor.
		virtual ~magnetic_B();

		// OTHERS

		void compute_force(const free_particle* p, math::vec3& F);
};

} // -- namespace fields
} // -- namespace physim
