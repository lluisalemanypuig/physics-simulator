#pragma once

// physim includes
#include <physim/fields/punctual.hpp>
#include <physim/particles/particle.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace fields {

/**
 * @brief Gravitational field.
 *
 * This class implements the force that a gravitational field
 * applies on a particle of mass m.
 *
 * This gravitational field is caused by some body of mass
 * @ref M, at position @ref puntual::pos.
 *
 * Therefore the force is computed using
 * \f$-G \cdot M \cdot m \cdot \frac{p - O}{||p - O||}\f$
 * where
 * - \f$G\f$ is the gravitational constant.
 * - \f$M\f$ is the mass of the particle causing the field.
 * - \f$p\f$ is the particle's position.
 * - \f$O\f$ is the position of the particle causing the field.
 */
class gravitational : public punctual {
	protected:
		/// Mass of object causing the gravitational field. [Kg]
		float M;

	public:
		/// Default constructor.
		gravitational();
		/// Constructor with position and mass.
		gravitational(const math::vec3& pos, float M);
		/// Copy constructor.
		gravitational(const gravitational& f);
		/// Destructor.
		virtual ~gravitational();

		// SETTERS

		/// Sets this gravitational field's mass. See @ref M.
		void set_mass(float M);

		// GETTERS

		/// Returns this field's mass. See @ref M.
		float get_mass() const;

		// OTHERS

		void compute_force(const particle *p, math::vec3& F);
};

} // -- namespace fields
} // -- namespace physim
