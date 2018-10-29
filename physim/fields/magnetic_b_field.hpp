#pragma once

// physim includes
#include <physim/fields/magnetic_field.hpp>

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
class magnetic_B_field : public magnetic_field {
	protected:

	public:
		/// Default constructor
		magnetic_B_field();
		/// Constructor with position and magnetic field vector.
		magnetic_B_field(const math::vec3& pos, const math::vec3& b);
		/// Copy constructor.
		magnetic_B_field(const magnetic_B_field& f);
		/// Destructor.
		virtual ~magnetic_B_field();

		// OTHERS

		void compute_force(const particle* p, math::vec3& F);
};

} // -- namespace fields
} // -- namespace physim
