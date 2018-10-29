#pragma once

// physim includes
#include <physim/fields/field.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace fields {

class punctual_field : public field {
	protected:
		/// Center of the field vector. [m]
		math::vec3 pos;
	public:
		/**
		 * @brief Default constructor.
		 *
		 * Position @ref pos is initialised at (0,0,0).
		 */
		punctual_field();
		/// Constructor with position.
		punctual_field(const math::vec3& p);
		/// Copy constructor.
		punctual_field(const punctual_field& f);
		/// Destructor
		virtual ~punctual_field();

		// SETTERS

		/// Sets the position of the field. See @ref pos.
		void set_position(const math::vec3& p);

		// GETTERS

		/// Returns the position of the field. See @ref pos.
		const math::vec3& get_position() const;
};

} // -- namespace fields
} // -- namespace physim
