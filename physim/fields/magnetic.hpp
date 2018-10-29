#pragma once

// physim includes
#include <physim/particles/particle.hpp>
#include <physim/fields/punctual.hpp>
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
class magnetic : public punctual {
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
		magnetic();
		/// Constructor with magnetic field vector.
		magnetic(const math::vec3& pos, const math::vec3& b);
		/// Copy constructor.
		magnetic(const magnetic& f);
		/// Destructor.
		virtual ~magnetic();

		// SETTERS

		/// Sets this magnetic field vector. See @ref B.
		void set_vector(const math::vec3& v);

		// GETTERS

		/// Returns this field's magnetic vector. See @ref B.
		const math::vec3& get_vector() const;

};

} // -- namespace fields
} // -- namespace physim
