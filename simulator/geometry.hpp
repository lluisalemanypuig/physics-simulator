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
		 * @return Returns true if @e p is inside the geometry. Returns
		 * false if otherwise. Depending on the type of geometry this
		 * method has a different geometrical interpretation.
		 */
		virtual bool is_inside(const vec3& p) const = 0;

		/// Returns the type of geometry of this object.
		virtual geom_type get_geom_type() const;
};

} // -- namespace geom
} // -- namespace sim
