#pragma once

// glm includes
#include <glm/vec3.hpp>
using namespace glm;

namespace sim {
namespace geom {

/// Enumeration for the type of geometry.
enum geom_type {
	none = -1,
	Plane, Triangle, Sphere
};

/**
 * @brief All geometrical on the scene
 *
 * This class is used to group all geometrical objects
 * on the simulated scene.
 *
 * These objects are typically spheres, planes, triangles
 * that are fixed at a certain position.
 *
 * Maybe they could also move, but not as a result of
 * any collision between this object and another one.
 */
class geometry {
	public:
		/// Default constructor
		geometry();
		/// Destructor
		virtual ~geometry();

		/**
		 * @brief Sets the position of the geometry.
		 *
		 * For each type of object that implements this class,
		 * setting the position has a different geometrical
		 * interpretation.
		 * @param p The new position of the object.
		 */
		virtual void set_position(const vec3& p) = 0;

		/**
		 * @brief Returns whether a point is inside the geometry.
		 * @param p The point to check whether it is inside the geometry.
		 * @param tol Optional tolerance to deal corerctly with equalities.
		 * @return Returns true if @e p is inside the geometry. Returns
		 * false if otherwise. Depending on the type of geometry this
		 * method has a different geometrical interpretation.
		 */
		virtual bool is_inside(const vec3& p, float tol = 1.e-7f) const = 0;

		/**
		 * @brief Returns true if the segment [@e p1, @e p2 ] intersects with
		 * the geometry.
		 * @param p1 The first endpoint of the segment.
		 * @param p2 The second endpoint of the segment.
		 * @param p_inter The intersection point between the segment and the
		 * geometry.
		 * @return Returns true if the segment and the geometry intersect.
		 * In this case, the value in @e p_inter will be the intersection point.
		 */
		virtual bool intersec_segment(const vec3& p1, const vec3& p2, vec3& p_inter) const = 0;

		/// Returns the type of geometry of this object.
		virtual geom_type get_geom_type() const;
};

} // -- namespace geom
} // -- namespace sim
