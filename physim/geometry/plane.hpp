#pragma once

// C++ includes
#include <iostream>

// physim includes
#include <physim/geometry/geometry.hpp>
#include <physim/particles/free_particle.hpp>
#include <physim/particles/mesh_particle.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace geom {

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
		 * The plane's equation is
		 \verbatim
		 dot(n,X) + d = 0
		 \endverbatim
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
		 \verbatim
		 n = normalise( (p1 - p0) x (p2 - p0) )
		 \endverbatim
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

		geom_type get_geom_type() const;

		/// Returns the distance between point @e p and this plane.
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
		 * Returns true iff
		 \verbatim
		 normal**p + dconst = 0
		 \endverbatim
		 * where @e normal is @ref normal and @e dconst is
		 * @ref dconst.
		 *
		 * The equality, however, is satisfied when the left
		 * handside of the equation is smaller or equal than
		 * @e tol.
		 */
		bool is_inside(const math::vec3& p, float tol = 1.e-6f) const;

		/**
		 * @brief Returns if the segment defined by the points @e p1 and @e p2 intersects
		 * with the plane.
		 * @param[in] p1 First endpoint of the segment.
		 * @param[in] p2 Second endpoint of the segment.
		 * @return Returns true if there is intersection.
		 */
		bool intersec_segment
		(const math::vec3& p1, const math::vec3& p2) const;
		/**
		 * @brief Returns the intersection between this plane and the segment
		 * defined by the points @e p1 and @e p2.
		 * @param[in] p1 First endpoint of the segment.
		 * @param[in] p2 Second endpoint of the segment.
		 * @param[out] p_inter If the segment intersects the plane then
		 * this will contain the intersection point.
		 * @return Returns true if there is intersection. In this case, the value
		 * of @e p_inter is valid at the end of the method.
		 */
		bool intersec_segment
		(const math::vec3& p1, const math::vec3& p2, math::vec3& p_inter) const;

		// OTHERS

		void update_particle(
			const math::vec3& pp, const math::vec3& pv,
			particles::free_particle *p
		) const;
		void update_particle(
			const math::vec3& pred_pos, const math::vec3& pred_vel,
			size_t i, meshes::mesh *m
		) const;

		void display(std::ostream& os = std::cout) const;
};

} // -- namespace geom
} // -- namespace sim
