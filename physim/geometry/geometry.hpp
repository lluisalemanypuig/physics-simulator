#pragma once

// C++ includes
#include <iostream>

// physim includes
#include <physim/particles/particle.hpp>
#include <physim/math/vec3.hpp>

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
		virtual void set_position(const math::vec3& p) = 0;

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
		virtual bool is_inside(const math::vec3& p, float tol = 1.e-6f) const = 0;

		/**
		 * @brief Returns if the segment defined by the points @e p1
		 * and @e p2 intersects the geometry.
		 * @param[in] p1 First endpoint of the segment.
		 * @param[in] p2 Second endpoint of the segment.
		 * @return Returns true if there is intersection.
		 */
		virtual bool intersec_segment(const math::vec3& p1, const math::vec3& p2) const = 0;

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
		virtual bool intersec_segment
		(const math::vec3& p1, const math::vec3& p2, math::vec3& p_inter) const = 0;

		// OTHERS

		/**
		 * @brief Update a particle in a collision with geometry.
		 *
		 * Assumig that particle @e pred collided with this geometry, update
		 * its position, velocity, ... accordingly.
		 *
		 * For example, some geometry may be 'bouncy', it may give the particle
		 * some extra speed. This needs to be updated in this method.
		 *
		 * The particle @e pred is given in a state that is not modified by any
		 * solver. That is, the particle's position, velocity, force, ... have the
		 * value at time step @e T. The predicted position and velocity are given in
		 * @e pred_pos and @e pred_vel. These are predictions of the position and
		 * velocity for time @e T + @e dt (where dt is some time step). The modified
		 * particle due to the collision must be stored in @e pred.
		 *
		 * This method is called only when it is sure that there has been a
		 * collision: the segment joining the particle's current position and
		 * the predicted position intersects the geometry.
		 *
		 * @param[in] pred_pos The predicted position of the particle.
		 * @param[in] pred_vel The predicted velocity of the particle.
		 * @param[out] pred The particle with the result of the collision.
		 */
		virtual void update_upon_collision(
			const math::vec3& pred_pos, const math::vec3& pred_vel,
			particle *pred
		) const = 0;

		/// Output on stream @e os information about this geometry.
		virtual void display(std::ostream& os = std::cout) const = 0;
};

} // -- namespace geom
} // -- namespace sim
