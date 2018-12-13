#pragma once

// physim includes
#include <physim/geometry/geometry.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/particles/free_particle.hpp>
#include <physim/particles/sized_particle.hpp>
#include <physim/math/vec2.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace geometry {

/**
 * @brief Class that implements a triangle.
 *
 * A triangle is, informally, a polygonal object of three
 * sides of arbitrary length, whose endpoints are
 * defined by three vertices (see @ref p0, @ref p1, @ref p2).
 *
 * The normal of a triangle is then
 *
 * \f$ (p_1 - p_0) \times (p_2 - p_0) / || (p_1 - p_0) \times (p_2 - p_0) || \f$
 *
 * These three vertices all lie on a plane (see @ref pl),
 * the creation of which depends on the order of the vertices
 * they are given in.
 *
 * There is a lot more information in this triangle, motivated by the
 * sphere-triangle intersection test described in:
 *
 * https://www.geometrictools.com/Documentation/IntersectionMovingSphereTriangle.pdf
 */
class triangle : public geometry {
	private:
		/// The first vertex of the triangle.
		math::vec3 p0;
		/// The second vertex of the triangle.
		math::vec3 p1;
		/// The third vertex of the triangle.
		math::vec3 p2;

		/// Plane associated to the triangle.
		plane pl;

		/**
		 * @brief Plane parametrisation vector.
		 *
		 * \f$ \vec{u_0} = (p_1 - p_0)/||p_1 - p_2|| \f$
		 */
		math::vec3 u0;
		/**
		 * @brief Plane parametrisation vector.
		 *
		 * \f$ \vec{u_1} = \vec{u_2} \times \vec{u_0} \f$
		 */
		math::vec3 u1;
		/**
		 * @brief Plane parametrisation vector.
		 *
		 * \f$ \vec{u_2} =
		 *		(p_1-p_0)\times(p_2-p_0)/
		 *		|| (p_1-p_0)\times(p_2-p_0) || \f$
		 */
		math::vec3 u2;

		/**
		 * @brief Local reference system point.
		 *
		 * \f$q_0 = (0,0)\f$
		 */
		math::vec2 q0;
		/**
		 * @brief Local reference system point.
		 *
		 * \f$q_1 = (l,0), l = ||p_1-p_0||\f$
		 */
		math::vec2 q1;
		/**
		 * @brief Local reference system point.
		 *
		 * \f$q_2 = (\alpha,\beta)\f$,
		 *
		 * where
		 * \f$\alpha = u_0\cdot(p_2-p_0), \beta = u_1\cdot(p_2-p_0)\f$
		 */
		math::vec2 q2;

		/**
		 * @brief Local reference system edge vector.
		 *
		 * \f$\vec{e_0} = q_1 - q_0\f$
		 */
		math::vec2 e0;
		/**
		 * @brief Local reference system edge vector.
		 *
		 * \f$\vec{e_1} = q_2 - q_1\f$
		 */
		math::vec2 e1;
		/**
		 * @brief Local reference system edge vector.
		 *
		 * \f$\vec{e_2} = q_0 - q_2\f$
		 */
		math::vec2 e2;

		/**
		 * @brief Local reference system normal edge vector.
		 *
		 * Perpendicular to \f$e_0\f$, see @ref e0.
		 *
		 * \f$\vec{n_0} = (0,-1)\f$
		 */
		math::vec2 n0;
		/**
		 * @brief Local reference system normal edge vector.
		 *
		 * Perpendicular to \f$e_1\f$, see @ref e1.
		 *
		 * \f$\vec{n_1} =
		 *		(\beta, l - \alpha)/
		 *		|| (\beta, l - \alpha) || \f$
		 *
		 * where
		 * \f$\alpha = u_0\cdot(p_2-p_0), \beta = u_1\cdot(p_2-p_0)\f$
		 */
		math::vec2 n1;
		/**
		 * @brief Local reference system normal edge vector.
		 *
		 * Perpendicular to \f$e_2\f$, see @ref e2.
		 *
		 * \f$\vec{n_2} =
		 *		(-\beta, \alpha)/
		 *		|| (-\beta, \alpha) || \f$
		 *
		 * where
		 * \f$\alpha = u_0\cdot(p_2-p_0), \beta = u_1\cdot(p_2-p_0)\f$
		 */
		math::vec2 n2;

		/**
		 * @brief Local reference system normal edge point.
		 *
		 * \f$q_0 + \vec{n_0}\f$
		 */
		math::vec2 q0n0;
		/**
		 * @brief Local reference system normal edge point.
		 *
		 * \f$q_0 + \vec{n_2}\f$
		 */
		math::vec2 q0n2;
		/**
		 * @brief Local reference system normal edge point.
		 *
		 * \f$q_1 + \vec{n_0}\f$
		 */
		math::vec2 q1n0;
		/**
		 * @brief Local reference system normal edge point.
		 *
		 * \f$q_1 + \vec{n_1}\f$
		 */
		math::vec2 q1n1;
		/**
		 * @brief Local reference system normal edge point.
		 *
		 * \f$q_2 + \vec{n_1}\f$
		 */
		math::vec2 q2n1;
		/**
		 * @brief Local reference system normal edge point.
		 *
		 * \f$q_2 + \vec{n_2}\f$
		 */
		math::vec2 q2n2;

	private:

		/// Returns the are of this triangle.
		float get_area() const;

	public:
		/// Default constructor.
		triangle();
		/**
		 * @brief Constructor with points.
		 *
		 * The plane associated to this triangle (see @ref pl)
		 * is built using these vertices in the same order they
		 * are given in this method.
		 *
		 * See
		 * @ref plane::plane(const math::vec3&,const math::vec3&,const math::vec3&)
		 * to see how the normal is determined.
		 */
		triangle
		(const math::vec3& _p0,const math::vec3& _p1,const math::vec3& _p2);
		/// Copy constructor.
		triangle(const triangle& t);
		/// Destructor.
		~triangle();

		// OPERATORS

		// SETTERS

		/**
		 * @brief Set the points of this triangle.
		 *
		 * The plane associated to this triangle (see @ref pl)
		 * is built using these vertices in the same order they
		 * are given in this method.
		 *
		 * See
		 * @ref plane::plane(const math::vec3&,const math::vec3&,const math::vec3&)
		 * to see how the normal is determined.
		 */
		void set_points
		(const math::vec3& _p0,const math::vec3& _p1,const math::vec3& _p2);

		/**
		 * @brief Sets the position of this triangle.
		 *
		 * The vertices of the triangle are translated with vector @e v.
		 * @param v Vector representing the direction in which
		 * every vertex moves.
		 */
		void set_position(const math::vec3& v);

		// GETTERS

		/// Returns a constant reference to the assiociated plane (@ref pl).
		const plane& get_plane() const;

		/**
		 * @brief Return the points of this triangle.
		 * @param[out] _p0 First point of the triangle. See @ref p0.
		 * @param[out] _p1 Second point of the triangle. See @ref p1.
		 * @param[out] _p2 Third point of the triangle. See @ref p2.
		 */
		void get_points
		(math::vec3& _p0, math::vec3& _p1, math::vec3& _p2) const;

		/**
		 * @brief Project a point to this triangle.
		 *
		 * The projected point is the closest point inside the triangle
		 * to point @e p.
		 * @param[in] p Point to be projected onto the triangle.
		 * @param[out] proj Projection of @e p onto the closest point to it
		 * inside the triangle.
		 */
		void projection(const math::vec3& p, math::vec3& proj) const;

		/**
		 * @brief Computes the distance between a point and this triangle.
		 * @param[in] p Point to measure the distance to the triangle.
		 * @return Returns the distance between @e p and this triangle.
		 */
		float distance(const math::vec3& p) const;

		bool is_inside(const math::vec3& p, float tol = 1.e-6f) const;

		geometry_type get_geom_type() const;

		bool intersec_segment
		(const math::vec3& _p0, const math::vec3& _p1) const;
		bool intersec_segment
		(const math::vec3& _p0, const math::vec3& _p1, math::vec3& p_inter) const;

		bool intersec_sphere
		(const math::vec3& c, float R) const;

		// OTHERS

		void update_particle(
			const math::vec3& pp, const math::vec3& pv,
			particles::free_particle *p
		) const;

		void correct_position(
			const math::vec3& pred_pos, const particles::sized_particle *p,
			math::vec3& correct_pos
		) const;

		void update_particle(
			const math::vec3& pp, const math::vec3& pv,
			particles::sized_particle *p
		) const;

		void display() const;
};

} // -- namespace geom
} // -- namespace sim
