#pragma once

// physim includes
#include <physim/particles/particle.hpp>
#include <physim/fields/punctual_field.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace fields {

/**
 * @brief Punctual magnetic field.
 *
 * Described by a position @ref pos and a magentic
 * field vector @ref B.
 *
 * There are several types of magnetic fields.
 * As an example, see @ref magnetic_B_field.
 */
class magnetic_field : public punctual_field {
	protected:
		/// Magnetic field vector [T].
		math::vec3 B;

	public:
		/**
		 * @brief Default constructor.
		 *
		 * The magnetic field vector @ref B is initialised
		 * to (0,0,0).
		 */
		magnetic_field();
		/// Constructor with magnetic field vector.
		magnetic_field(const math::vec3& pos, const math::vec3& b);
		/// Copy constructor.
		magnetic_field(const magnetic_field& f);
		/// Destructor.
		virtual ~magnetic_field();

		// SETTERS

		/// Sets this magnetic field vector. See @ref B.
		void set_vector(const math::vec3& v);

		// GETTERS

		/// Returns this field's magnetic vector. See @ref B.
		const math::vec3& get_vector() const;

};

} // -- namespace fields
} // -- namespace physim
