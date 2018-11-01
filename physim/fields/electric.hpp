#pragma once

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/fields/punctual.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace fields {

class electric : public punctual {
	protected:
		/// Electric field charge. [C]
		float Q;

	public:
		/// Default constructor.
		electric();
		/// Constructor with position and electric field charge.
		electric(const math::vec3& pos, float Q);
		/// Copy constructor.
		electric(const electric& f);
		/// Destructor.
		virtual ~electric();

		// SETTERS

		/// Sets this electric field's charge. See @ref Q.
		void set_charge(float Q);

		// GETTERS

		/// Returns this field's charge. See @ref Q.
		float get_charge() const;

		// OTHERS

		void compute_force(const particles::free_particle *p, math::vec3& F);
};

} // -- namespace fields
} // -- namespace physim
