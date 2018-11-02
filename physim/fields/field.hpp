#pragma once

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/particles/mesh_particle.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace fields {

class field {
	private:
	protected:
	public:
		/// Default constructor.
		field();
		/// Copy constructor.
		field(const field& f);
		/// Destructor.
		virtual ~field();

		// OTHERS

		/**
		 * @brief Compute the force vector acting on a particle.
		 *
		 * The contents of parameter @e F is undefined when calling
		 * this function, thus the contents should be overwritten.
		 * @param[in] p Particle where the force is applied to.
		 * @param[out] F The force from this field acting on the
		 * particle.
		 */
		virtual void compute_force(const particles::free_particle *p, math::vec3& F) = 0;
		/**
		 * @brief Compute the force vector acting on a particle.
		 *
		 * The contents of parameter @e F is undefined when calling
		 * this function, thus the contents should be overwritten.
		 * @param[in] p Particle where the force is applied to.
		 * @param[out] F The force from this field acting on the
		 * particle.
		 */
		virtual void compute_force(const particles::mesh_particle *p, math::vec3& F) = 0;
};

} // -- namespace fields
} // -- namespace physim
