#pragma once

// glm includes
#include <glm/glm.hpp>
using namespace glm;

// Custom includes
#include <simulator/geometry/geometry.hpp>

namespace sim {
namespace geom {

/**
 * @brief Class that implements a plane.
 */
class plane : public geometry {
	private:
		/**
		 * @brief Normal of the plane.
		 *
		 * This is a unit vector.
		 */
		vec3 normal;
		/// Constant, independent term of the plane equation.
		float dconst;

	public:
		/// Default constructor.
		plane();
		/**
		 * @brief Construct a plane with a point and a normal.
		 * @param p Point.
		 * @param n Normal vector of the plane.
		 * @post @ref normal takes the normalisation of the vector @e n.
		 */
		plane(const vec3& p, const vec3& n);
		/// Construct plane with three points.
		plane(const vec3& p0, const vec3& p1, const vec3& p2);
		/// Destructor.
		~plane();

		// SETTERS

		/**
		 * @brief Sets the position of this plane.
		 *
		 * Modifies @ref dconst so that the plane's equation is satisifed.
		 * @param p Point.
		 */
		void set_position(const vec3& p);

		// GETTERS

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
		bool is_inside(const vec3& p, float tol = 1.e-7f) const;
		geom_type get_geom_type() const;

		/// Returns the distance between point @e p and this plane.
		float dist_point_plane(const vec3& p) const;
		/// Returns the project of point @e p onto the plane.
		vec3 closest_point_plane(const vec3& p) const;
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
		bool intersec_segment(const vec3& p1, const vec3& p2, vec3& p_inter) const;

};

} // -- namespace geom
} // -- namespace sim
