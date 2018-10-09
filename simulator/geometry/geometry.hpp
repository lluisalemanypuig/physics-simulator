#pragma once

// C++ includes
#include <iostream>
using namespace std;

// glm includes
#include <glm/vec3.hpp>
using namespace glm;

// simulator includes
#include <simulator/particle.hpp>

namespace physim {
namespace geom {

/// Enumeration for the type of geometry.
enum geom_type {
	none = -1,
	Plane, Triangle,
	Rectangle, Sphere
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
		/// Copy constructor
		geometry(const geometry& g);
		/// Destructor
		virtual ~geometry();

		// SETTERS

		/**
		 * @brief Sets the position of the geometry.
		 *
		 * For each type of object that implements this class,
		 * setting the position has a different geometrical
		 * interpretation.
		 * @param p The "new position" of the object.
		 */
		virtual void set_position(const vec3& p) = 0;

		// GETTERS

		/// Returns the type of geometry of this object.
		virtual geom_type get_geom_type() const;

		/**
		 * @brief Returns whether a point is inside the geometry.
		 * @param p The point to check whether it is inside the geometry.
		 * @param tol Optional tolerance to deal corerctly with equalities.
		 * @return Returns true if @e p is inside the geometry. Returns
		 * false if otherwise. Depending on the type of geometry this
		 * method has a different geometrical interpretation.
		 */
		virtual bool is_inside(const vec3& p, float tol = 1.e-6f) const = 0;

		/**
		 * @brief Returns if the segment defined by the points @e p1
		 * and @e p2 intersects the geometry.
		 * @param[in] p1 First endpoint of the segment.
		 * @param[in] p2 Second endpoint of the segment.
		 * @return Returns true if there is intersection.
		 */
		virtual bool intersec_segment(const vec3& p1, const vec3& p2) const = 0;

		/**
		 * @brief Returns true if the segment [@e p1, @e p2 ] intersects with
		 * the geometry.
		 * @param[in] p1 The first endpoint of the segment.
		 * @param[in] p2 The second endpoint of the segment.
		 * @param[out] p_inter The intersection point between the segment and the
		 * geometry.
		 * @return Returns true if the segment and the geometry intersect.
		 * In this case, the value in @e p_inter will be the intersection point.
		 */
		virtual bool intersec_segment(const vec3& p1, const vec3& p2, vec3& p_inter) const = 0;

		// OTHERS

		/**
		 * @brief Update a particle collision in a collision with geometry.
		 *
		 * Assumig that particle @e p collided with this geometry, update its
		 * position accordingly.
		 *
		 * For example, some geometry may be 'bouncy', the may give the particle
		 * some extra speed. This needs to be updated in this method.
		 *
		 * The particle is given in a state modified by a solver. That is,
		 * the particle's position, velocity, force, ... have the value
		 * calculated by a solver after applying the current time step.
		 *
		 * This method is called only when it is sure that there has
		 * been a collision: the segment joining the particle's previous
		 * position and the current position intersects the geometry.
		 *
		 * @param p The particle to be updated.
		 */
		virtual void update_upon_collision(particle *p) const = 0;

		/// Output on stream @e os information about this geometry.
		virtual void display(ostream& os = cout) const = 0;
};

} // -- namespace geom
} // -- namespace sim
