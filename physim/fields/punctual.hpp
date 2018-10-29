#pragma once

// physim includes
#include <physim/fields/field.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace fields {

/**
 * @brief Force field caused by a particle.
 */
class punctual : public field {
	protected:
		/// Center of the field vector. [m]
		math::vec3 pos;
	public:
		/**
		 * @brief Default constructor.
		 *
		 * Position @ref pos is initialised at (0,0,0).
		 */
		punctual();
		/// Constructor with position.
		punctual(const math::vec3& p);
		/// Copy constructor.
		punctual(const punctual& f);
		/// Destructor
		virtual ~punctual();

		// SETTERS

		/// Sets the position of the field. See @ref pos.
		void set_position(const math::vec3& p);

		// GETTERS

		/// Returns the position of the field. See @ref pos.
		const math::vec3& get_position() const;
};

} // -- namespace fields
} // -- namespace physim
