#pragma once

// glm includes
#include <glm/glm.hpp>
using namespace glm;

// Custom includes
#include <simulator/geometry/geometry.hpp>
#include <simulator/geometry/plane.hpp>
#include <simulator/particle.hpp>

namespace physim {
namespace geom {

/**
 * @brief Class that implements a sphere.
 */
class sphere : public geometry {
	private:
		/// Centre of the sphere.
		vec3 C;
		/// Radius of the sphere.
		float R;

	public:
		/// Default constructor.
		sphere();
		/// Constructor with centre and radius.
		sphere(const vec3& c, float r);
		/// Copy constructor.
		sphere(const sphere& s);
		/// Destructor.
		~sphere();

		// SETTERS

		/**
		 * @brief Sets the position of this sphere.
		 *
		 * Its center is set to @e c.
		 * @param c New center of the sphere.
		 */
		void set_position(const vec3& c);

		/// Sets the radius of the sphere.
		void set_radius(float r);

		// GETTERS

		/// Returns the centre of the shere.
		const vec3& get_centre() const;
		/// Returns the radius of the sphere.
		float get_radius() const;

		bool is_inside(const vec3& p, float tol = 1.e-6f) const;
		geom_type get_geom_type() const;

		bool intersec_segment(const vec3& p1, const vec3& p2) const;
		bool intersec_segment(const vec3& p1, const vec3& p2, vec3& p_inter) const;

		// OTHERS

		void update_upon_collision(particle *p) const;
		void display(ostream& os = cout) const;
};

} // -- namespace geom
} // -- namespace sim
