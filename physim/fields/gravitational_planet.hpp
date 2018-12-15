#pragma once

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/particles/mesh_particle.hpp>
#include <physim/fields/punctual.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace fields {

/**
 * @brief A planet's gravitational field.
 *
 * This class implements the gravitational force field
 * typical of a planet.
 *
 * This field is assumed to have some huge mass, placed
 * at some point near infinity in a certain direction.
 * This vector is stored at the position to which this
 * field is set to, and interpreted as the acceleration
 * of the gravity it yields. Thus, the force it applies
 * on a particle of mass \f$m\f$ is:
 *
 * \f$\vec{F} = m\cdot \vec{g}\f$
 *
 * Here \f$g\f$ is the vector set in @ref punctual::pos.
 */
class gravitational_planet : public punctual {
	private:
		/**
		 * @brief Function that actuall computes the force of this field.
		 *
		 * Works for @ref particles::free_particle and
		 * @ref particles::mesh_particle.
		 */
		template<class P>
		void __compute_force(const P *p, math::vec3& F);

	protected:

	public:
		/// Default constructor.
		gravitational_planet();
		/// Constructor with position and mass.
		gravitational_planet(const math::vec3& pos);
		/// Copy constructor.
		gravitational_planet(const gravitational_planet& f);
		/// Destructor.
		virtual ~gravitational_planet();

		// SETTERS

		// OTHERS

		void compute_force(const particles::free_particle *p, math::vec3& F);
		void compute_force(const particles::mesh_particle *p, math::vec3& F);
};

} // -- namespace fields
} // -- namespace physim
