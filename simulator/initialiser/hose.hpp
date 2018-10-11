#pragma once

// C++ includes
#include <random>
using namespace std;

// glm includes
#include <glm/glm.hpp>
using namespace glm;

// Custom includes
#include <simulator/initialiser/initialiser.hpp>

namespace physim {
namespace init {

/**
 * @brief A hose source class initialiser.
 *
 * NEEDS DESCRIPTION
 */
class hose : public initialiser {
	protected:
		/// Engine used in the uniform distribution @ref U01.
		default_random_engine E;
		/// Random number generator for uniform values between 0 and 1.
		uniform_real_distribution<float> U01;

		/// The source point of the cone.
		vec3 source;
		/// The center of the cone's base.
		vec3 cc;
		/// Unit vector along the height.
		vec3 u;

		/**
		 * @brief Unit vector on the circle.
		 *
		 * It is perpendicular to @ref w.
		 */
		vec3 v;
		/**
		 * @brief Unit vector on the circle.
		 *
		 * It is perpendicular to @ref v.
		 */
		vec3 w;

		/// Radius of the cone.
		float r;
		/// Height of the cone.
		float h;

	protected:
		/**
		 * @brief Sets the position initialser.
		 *
		 * It is made a virtual function so that this class can be
		 * reimplemented in another one.
		 *
		 * Positions are generated according to the parametrisation
		 * of this rectangle.
		 */
		virtual void make_pos_init();
		/**
		 * @brief Sets the velocity initialser.
		 *
		 * It is made a virtual function so that this class can be
		 * reimplemented in another one.
		 */
		virtual void make_vel_init();

	public:
		/// Default constructor.
		hose();
		/**
		 * @brief Copy constructor.
		 *
		 * The functions @ref initialiser::pos and @ref initialser::vel
		 * are not copied. Instead, they are remade (functions
		 * @ref make_pos_init and @ref make_vel_init are called again).
		 */
		hose(const hose& h);
		/// Destructor.
		virtual ~hose();

		// SETTERS

		/**
		 * @brief Sets the source of the hose.
		 * @param _S See @ref S.
		 * @param _u See @ref u.
		 * @param _r See @ref r.
		 * @param _h See @ref h.
		 */
		void set_hose_source(const vec3& _S, const vec3& _u, float _r,float _h);

		// GETTERS

		initialiser *clone() const;

};

} // -- namespace init
} // -- namespace physim
