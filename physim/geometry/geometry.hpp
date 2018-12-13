#pragma once

// C++ includes

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/particles/sized_particle.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace geometry {

/// Enumeration for the type of geometry.
enum geometry_type {
	none = -1,
	/// See @ref geometry::plane for details.
	Plane,
	/// See @ref geometry::triangle for details.
	Triangle,
	/// See @ref geometry::rectangle for details.
	Rectangle,
	/// See @ref geometry::sphere for details.
	Sphere,
	/// See @ref geometry::object for details.
	Object
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
	private:
	protected:
		/// Minimum coordinates of the geometry.
		math::vec3 vmin;
		/// Maximum coordinates of the geometry.
		math::vec3 vmax;
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
		virtual geometry_type get_geom_type() const;

		/**
		 * @brief Returns whether a point is inside the geometry.
		 * @param p The point to check whether it is inside the geometry.
		 * @param tol Optional tolerance to deal corerctly with equalities.
		 * @return Returns true if @e p is inside the geometry. Returns
		 * false if otherwise. Depending on the type of geometry this
		 * method has a different geometrical interpretation.
		 */
		virtual bool is_inside
		(const math::vec3& p, float tol = 1.e-6f) const = 0;

		/**
		 * @brief Returns if the segment defined by the points @e p1
		 * and @e p2 intersects the geometry.
		 * @param[in] p1 First endpoint of the segment.
		 * @param[in] p2 Second endpoint of the segment.
		 * @return Returns true if there is intersection.
		 */
		virtual bool intersec_segment
		(const math::vec3& p1, const math::vec3& p2) const = 0;

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

		/**
		 * @brief Returns if the sphere with center the point @e c and radius
		 * @e R intersects the geometry.
		 * @param[in] c Center of the sphere.
		 * @param[in] R radius of the sphere.
		 * @return Returns true if there is intersection.
		 */
		virtual bool intersec_sphere
		(const math::vec3& c, float R) const = 0;

		// OTHERS

		/**
		 * @brief Update a free particle in a collision with geometry.
		 *
		 * Assumig that particle @e pred collided with this geometry, update
		 * its position, velocity, ... accordingly.
		 *
		 * For example, some geometry may be 'bouncy', it may give the particle
		 * some extra speed. This needs to be updated in this method.
		 *
		 * The particle @e pred is given in a state that is not modified by any
		 * solver. That is, the particle's position, velocity, force, ... have
		 * the value at time step @e T. The predicted position and velocity are
		 * given in @e pred_pos and @e pred_vel. These are predictions of the
		 * position and velocity for time @e T + @e dt (where dt is some time
		 * step). The modified particle due to the collision must be stored in
		 * @e pred.
		 *
		 * This method is called only when it is sure that there has been a
		 * collision: the segment joining the particle's current position and
		 * the predicted position intersects the geometry.
		 *
		 * @param[in] pred_pos The predicted position of the particle.
		 * @param[in] pred_vel The predicted velocity of the particle.
		 * @param[out] pred The particle with the result of the collision.
		 */
		virtual void update_particle(
			const math::vec3& pred_pos, const math::vec3& pred_vel,
			particles::free_particle *pred
		) const = 0;

		/**
		 * @brief Corrects the position of a sized particle.
		 *
		 * Assuming that a sized particle at position @e pred_pos and radius
		 * @e R intersects the geometry, update the position of the particle
		 * to @e correct_pos, so that it no longer intersects the geometry but
		 * that is as closest as possible to it. The correction is done in
		 * the direction of the velocity vector.
		 * @param[in] pred_pos Position predicted by a solver.
		 * @param[in] p Current state of the particle.
		 * @param[out] correct_pos Correction of the particle's position.
		 * @pre The sized particle intersects the geometry.
		 */
		virtual void correct_position
		(const math::vec3& pred_pos, const particles::sized_particle *p,
		 math::vec3& correct_pos) const = 0;

		/**
		 * @brief Update a sized particle in a collision with geometry.
		 *
		 * Assumig that particle @e pred collided with this geometry, update
		 * its position, velocity, ... accordingly.
		 *
		 * For example, some geometry may be 'bouncy', it may give the particle
		 * some extra speed. This needs to be updated in this method.
		 *
		 * The particle @e pred is given in a state that is not modified by any
		 * solver. That is, the particle's position, velocity, force, ... have
		 * the value at time step @e T. The predicted position and velocity are
		 * given in @e pred_pos and @e pred_vel. These are predictions of the
		 * position and velocity for time @e T + @e dt (where dt is some time
		 * step). The modified particle due to the collision must be stored in
		 * @e pred.
		 *
		 * This method is called only when it is sure that there has been a
		 * collision: the segment joining the particle's current position and
		 * the predicted position intersects the geometry.
		 *
		 * @param[in] pred_pos The predicted position of the particle.
		 * @param[in] pred_vel The predicted velocity of the particle.
		 * @param[out] pred The particle with the result of the collision.
		 */
		virtual void update_particle(
			const math::vec3& pred_pos, const math::vec3& pred_vel,
			particles::sized_particle *pred
		) const = 0;

		/// Output on stream @e cout information about this geometry.
		virtual void display() const = 0;
};

} // -- namespace geom
} // -- namespace sim
