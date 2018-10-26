#pragma once

// C++ includes
#include <random>
using namespace std;

// physim includes
#include <physim/initialiser/initialiser.hpp>
#include <physim/particles/particle.hpp>
#include <physim/math/math.hpp>

namespace physim {
namespace init {

/**
 * @brief A rectangular source class initialiser.
 *
 * Provides a position initialiser function so that particles
 * are generated within a rectangle of specific dimensions.
 *
 * Need to set the source size through @ref set_rectangular_source
 * so that the initialser's source is defined. In this method
 * is specified the coordinates of one of its corners (the source
 * @ref S), the two unit vectors spanning the plane it belongs to
 * (see @ref u and @ref v), and the lengths of the sides, width and
 * length (see @ref h and @ref w).
 *
 * The points in this rectangle are, then, parametrised by:
 *
 * \f$R(\lambda, \mu) = S + \lambda\cdot w\cdot \vec{u} + \mu\cdot h\cdot \vec{u}\f$
 *
 * for \f$\lambda,\mu \in [0,1]\subset R\f$.
 *
 * Needless to say that if the vectors @ref u and @ref v are not
 * perpendicular the result will be a parallelogram, not a rectangle.
 * However, the word 'rectangle' will be used regardless of how
 * misleading it might be.
 */
class rect_source : public initialiser {
	protected:
		/// Engine used in the uniform distribution @ref U01.
		default_random_engine E;
		/// Random number generator for uniform values between 0 and 1.
		uniform_real_distribution<float> U01;

		/// The source point of the rectangle.
		math::vec3 S;
		/// The center of the rectangle.
		math::vec3 C;

		/// Unitary vector along the width.
		math::vec3 u;
		/// Unitary vector along the height.
		math::vec3 v;
		/**
		 * @brief Normal to the rectangle.
		 *
		 * Unitary vector. Cross-product of @ref u and @ref v.
		 */
		math::vec3 n;

		/// The distance between @ref S and the farthest point in \f$R(\lambda,0)\f$
		float w;
		/// The distance between @ref S and the farthest point in \f$R(0,\mu)\f$
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
		virtual void make_vel_init() = 0;

	public:
		/// Default constructor.
		rect_source();
		/**
		 * @brief Copy constructor.
		 *
		 * The function @ref initialiser::pos is not copied.
		 * Instead, it is remade (function @ref make_pos_init is
		 * called again).
		 */
		rect_source(const rect_source& rs);
		/// Destructor.
		virtual ~rect_source();

		// SETTERS

		/**
		 * @brief Sets the source of the rectangle.
		 *
		 * The order of @e _u and @e _v is important. It defines
		 * the 'above' and 'below' parts of the rectangle. This is
		 * user-defined.
		 *
		 * The normal (see @ref n) is determined as the cross-product
		 * of _u and _v, in this order.
		 * @param _S See @ref S.
		 * @param _u See @ref u.
		 * @param _v See @ref v.
		 * @param _w See @ref w.
		 * @param _h See @ref h.
		 */
		void set_rectangular_source(const math::vec3& _S, const math::vec3& _u,const math::vec3& _v, float _w,float _h);

		/**
		 * @brief Patricular case of flat rectangular source.
		 *
		 * Unitary vectors used are:
		 * - @ref u = (1.0, 0.0, 0.0)
		 * - @ref v = (0.0, 0.0, 1.0)
		 * @param _S See @ref S.
		 * @param _w See @ref w.
		 * @param _h See @ref h.
		 */
		void set_straight_source(const math::vec3& _S, float _w,float _h);

		// GETTERS

		virtual initialiser *clone() const = 0;

};

} // -- namespace init
} // -- namespace physim
