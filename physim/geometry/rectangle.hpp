/*********************************************************************
 * Real-time physics simulation project
 * Copyright (C) 2018-2019 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

#pragma once

// C++ includes

// physim includes
#include <physim/geometry/geometry.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/particles/free_particle.hpp>
#include <physim/particles/sized_particle.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace geometric {

/**
 * @brief Class that implements a rectangle.
 *
 * A rectangle is, informally, a polygonal object of four
 * sides of same opposite length, whose endpoints are
 * defined by four vertices (see @ref p0, @ref p1, @ref p2, @ref p3).
 *
 * These four vertices all lie on a plane (see @ref pl),
 * the creation of which depends on the order of the first
 * three vertices they are given in.
 */
class rectangle : public geometry {
	private:
		/// The first vertex of the rectangle.
		math::vec3 p0;
		/// The second vertex of the rectangle.
		math::vec3 p1;
		/// The third vertex of the rectangle.
		math::vec3 p2;
		/// The fourth vertex of the rectangle.
		math::vec3 p3;

		/// Plane associated to the rectangle.
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
		 *		( (p_1-p_0)\times(p_2-p_0) )/
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
		 * @brief Local reference system point.
		 *
		 * \f$q_3 = (\gamma,\delta)\f$,
		 *
		 * where
		 * \f$\gamma = u_0\cdot(p_3-p_0), \delta = u_1\cdot(p_3-p_0)\f$
		 */
		math::vec2 q3;

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
		 * \f$\vec{e_2} = q_3 - q_2\f$
		 */
		math::vec2 e2;
		/**
		 * @brief Local reference system edge vector.
		 *
		 * \f$\vec{e_3} = q_0 - q_3\f$
		 */
		math::vec2 e3;

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
		 *		(\delta - \beta, \alpha - \gamma)/
		 *		|| (\delta - \beta, \alpha - \gamma) || \f$
		 *
		 * where
		 * \f$\alpha = u_0\cdot(p_2-p_0), \beta = u_1\cdot(p_2-p_0)\f$,
		 * \f$\gamma = u_0\cdot(p_3-p_0), \delta = u_1\cdot(p_3-p_0)\f$
		 */
		math::vec2 n2;
		/**
		 * @brief Local reference system normal edge vector.
		 *
		 * Perpendicular to \f$e_3\f$, see @ref e3.
		 *
		 * \f$\vec{n_3} =
		 *		(-\delta, \gamma)/
		 *		|| (-\delta, \gamma) ||\f$
		 *
		 * where
		 * \f$\gamma = u_0\cdot(p_3-p_0), \delta = u_1\cdot(p_3-p_0)\f$
		 */
		math::vec2 n3;

		/**
		 * @brief Local reference system normal edge point.
		 *
		 * \f$q_0 + \vec{n_0}\f$
		 */
		math::vec2 q0n0;
		/**
		 * @brief Local reference system normal edge point.
		 *
		 * \f$q_0 + \vec{n_3}\f$
		 */
		math::vec2 q0n3;
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
		/**
		 * @brief Local reference system normal edge point.
		 *
		 * \f$q_3 + \vec{n_2}\f$
		 */
		math::vec2 q3n2;
		/**
		 * @brief Local reference system normal edge point.
		 *
		 * \f$q_3 + \vec{n_3}\f$
		 */
		math::vec2 q3n3;

	public:
		/// Default constructor.
		rectangle();
		/**
		 * @brief Constructor with points.
		 *
		 * The plane associated to this rectangle (see @ref pl)
		 * is built using the first three vertices in the same
		 * order they are given in this method.
		 *
		 * See
		 * @ref plane::plane(const math::vec3&,const math::vec3&,const math::vec3&)
		 * to see how the normal is determined.
		 *
		 * The four points must lie on the same plane, that is,
		 * the fourth point must lie on the plane made with the
		 * first three vertices.
		 */
		rectangle(
			const math::vec3& p1,const math::vec3& p2,
			const math::vec3& p3,const math::vec3& p4
		);
		/// Copy constructor.
		rectangle(const rectangle& t);
		/// Destructor.
		~rectangle();

		// OPERATORS

		// SETTERS

		/**
		 * @brief Set the points of this rectangle.
		 *
		 * The plane associated to this rectangle (see @ref pl)
		 * is built using these vertices in the same order they
		 * are given in this method.
		 *
		 * See
		 * @ref plane::plane(const math::vec3&,const math::vec3&,const math::vec3&)
		 * to see how the normal is determined.
		 */
		void set_points
		(const math::vec3& _p0,const math::vec3& _p1,
		 const math::vec3& _p2,const math::vec3& _p3);

		/**
		 * @brief Sets the position of this rectangle.
		 *
		 * The vertices of the rectangle are translated according
		 * to vector @e v.
		 * @param v Vector representing the direction in which
		 * every vertex moves.
		 */
		void set_position(const math::vec3& v);

		// GETTERS

		/// Returns a constant reference to the assiociated plane (@ref pl).
		const plane& get_plane() const;

		/**
		 * @brief Return the points of this rectangle.
		 * @param[out] _p0 First point of the rectangle. See @ref p0.
		 * @param[out] _p1 Second point of the rectangle. See @ref p1.
		 * @param[out] _p2 Third point of the rectangle. See @ref p2.
		 * @param[out] _p3 Fourth point of the rectangle. See @ref p3.
		 */
		void get_points
		(math::vec3& _p0, math::vec3& _p1, math::vec3& _p2, math::vec3& _p3)
		const;

		/**
		 * @brief Project a point onto this rectangle.
		 *
		 * The projected point is the closest point inside the rectangle
		 * to point @e p.
		 * @param[in] p Point to be projected onto the rectangle.
		 * @param[out] proj Projection of @e p onto the closest point to it
		 * inside the rectangle.
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
		(const math::vec3& _p1, const math::vec3& _p2) const;
		bool intersec_segment
		(const math::vec3& _p1, const math::vec3& _p2, math::vec3& p_inter) const;

		bool intersec_sphere(const math::vec3& c, float R) const;

		// OTHERS

		void update_particle(
			const math::vec3& pp, const math::vec3& pv,
			particles::free_particle& p
		) const;

		void update_particle(
			const math::vec3& pp, const math::vec3& pv,
			particles::sized_particle& p
		) const;

		void display() const;
};

} // -- namespace geom
} // -- namespace sim
