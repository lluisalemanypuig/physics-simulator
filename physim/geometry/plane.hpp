#pragma once

// C++ includes

// physim includes
#include <physim/geometry/geometry.hpp>
#include <physim/particles/free_particle.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace geometry {

/**
 * @brief Class that implements a plane.
 *
 * A plane is defined with a normal, unit vector
 * (see @ref normal) and a constant term (see @ref dconst).
 *
 * A plane can be defined only with these two parameters
 * (see @ref plane(const math::vec3&, const math::vec3&) or with three
 * points (see @ref plane(const math::vec3&, const math::vec3&, const math::vec3&)).
 */
class plane : public geometry {
	private:
		/**
		 * @brief Normal of the plane.
		 *
		 * This is a unit vector.
		 */
		math::vec3 normal;
		/// Constant, independent term of the plane equation.
		float dconst;

	public:
		/// Default constructor.
		plane();
		/**
		 * @brief Construct a plane with a normal and a point.
		 * @param n Normal vector of the plane.
		 * @param p Point.
		 * @post @ref normal takes the normalisation of the vector @e n.
		 */
		plane(const math::vec3& n, const math::vec3& p);
		/**
		 * @brief Construct a plane with a normal and the equation's independent term.
		 *
		 * The plane's equation is \f$ n \cdot X + d = 0\f$.
		 * @param n Normal vector of the plane.
		 * @param d Independent term of the plane's equation.
		 * @post @ref normal takes the normalisation of the vector @e n.
		 */
		plane(const math::vec3& n, float d);
		/**
		 * @brief Construct plane with three points.
		 *
		 * The normal of the plane is defined as the cross product
		 * of the vectors from @e p0 to @e p1 and from @e p0 to @e p2:
		 * \f$n = ( (p1 - p0) \times (p2 - p0) )/||(p1 - p0) \times (p2 - p0)|| \f$.
		 */
		plane(const math::vec3& p0, const math::vec3& p1, const math::vec3& p2);
		/// Copy constructor.
		plane(const plane& p);
		/// Destructor.
		~plane();

		// OPERATORS

		// SETTERS

		/**
		 * @brief Sets the position of this plane.
		 *
		 * Modifies @ref dconst so that the plane's equation is
		 * satisifed by point @e p.
		 * @param p Force the plane to go through this point.
		 */
		void set_position(const math::vec3& p);

		// GETTERS

		geometry_type get_geom_type() const;

		/**
		 * @brief Returns the signed distance between point @e p and this plane.
		 * @param p Point
		 * @return Returns a positive distance if the point @e p is at the
		 * 'positive' side of the plane, and a negative distance if the point
		 * @e p is at the 'negative' side of the plane.\n
		 * The positive side is that halfspace such that all the points \f$q\f$
		 * belonging to it are such that \f$ q\cdot n + d > 0\f$, where \f$n,d\f$
		 * are the normal vector and the constant term, respectively, of this
		 * plane.
		 */
		float dist_point_plane(const math::vec3& p) const;
		/// Returns the project of point @e p onto the plane.
		void closest_point_plane(const math::vec3& p, math::vec3& c) const;
		/// Returns the normal of this plane.
		const math::vec3& get_normal() const;
		/// Returns the independent term of this plane's equation.
		float get_constant() const;

		/**
		 * @brief Returns true if @e p satisfies this plane's equation.
		 *
		 * Returns true iff \f$normal \cdot p + dconst = 0\f$
		 * where @e normal is @ref normal and @e dconst is
		 * @ref dconst.
		 *
		 * The equality, however, is satisfied when the left
		 * handside of the equation is smaller or equal than
		 * @e tol.
		 */
		bool is_inside(const math::vec3& p, float tol = 1.e-6f) const;

		bool intersec_segment
		(const math::vec3& p1, const math::vec3& p2) const;
		bool intersec_segment
		(const math::vec3& p1, const math::vec3& p2, math::vec3& p_inter) const;

		bool intersec_sphere(const math::vec3& c, float R) const;

		// OTHERS

		void update_particle(
			const math::vec3& pp, const math::vec3& pv,
			particles::free_particle *p
		) const;

		void update_particle(
			const math::vec3& pp, const math::vec3& pv,
			particles::sized_particle *p
		) const;

		void display() const;
};

} // -- namespace geom
} // -- namespace sim
