#pragma once

// physim includes
#include <physim/fields/punctual_field.hpp>
#include <physim/particles/particle.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace fields {

class electric_field : public punctual_field {
	protected:
		/// Electric field charge. [C]
		float Q;

	public:
		/// Default constructor.
		electric_field();
		/// Constructor with position and electric field charge.
		electric_field(const math::vec3& pos, float Q);
		/// Copy constructor.
		electric_field(const electric_field& f);
		/// Destructor.
		virtual ~electric_field();

		// SETTERS

		/// Sets this electric field's charge. See @ref Q.
		void set_charge(float Q);

		// GETTERS

		/// Returns this field's charge. See @ref Q.
		float get_charge() const;

		// OTHERS

		void compute_force(const particle *p, math::vec3& F);
};

} // -- namespace fields
} // -- namespace physim
