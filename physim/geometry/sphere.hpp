#pragma once

// C++ includes
#include <iostream>

// physim includes
#include <physim/geometry/geometry.hpp>
#include <physim/particles/particle.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace geom {

/**
 * @brief Class that implements a sphere.
 */
class sphere : public geometry {
	private:
		/// Centre of the sphere.
		math::vec3 C;
		/// Radius of the sphere.
		float R;

	public:
		/// Default constructor.
		sphere();
		/// Constructor with centre and radius.
		sphere(const math::vec3& c, float r);
		/// Copy constructor.
		sphere(const sphere& s);
		/// Destructor.
		~sphere();

		// OPERATORS

		// SETTERS

		/**
		 * @brief Sets the position of this sphere.
		 *
		 * Its center is set to @e c.
		 * @param c New center of the sphere.
		 */
		void set_position(const math::vec3& c);

		/// Sets the radius of the sphere.
		void set_radius(float r);

		// GETTERS

		/// Returns the centre of the shere.
		const math::vec3& get_centre() const;
		/// Returns the radius of the sphere.
		float get_radius() const;

		bool is_inside(const math::vec3& p, float tol = 1.e-6f) const;
		geom_type get_geom_type() const;

		bool intersec_segment
		(const math::vec3& p1, const math::vec3& p2) const;
		bool intersec_segment
		(const math::vec3& p1, const math::vec3& p2, math::vec3& p_inter) const;

		// OTHERS

		void update_upon_collision
		(const math::vec3& pred_pos, const math::vec3& pred_vel, particle *p) const;

		void display(std::ostream& os = std::cout) const;
};

} // -- namespace geom
} // -- namespace sim
