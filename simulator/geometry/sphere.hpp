#pragma once

// glm includes
#include <glm/glm.hpp>
using namespace glm;

// Custom includes
#include <simulator/geometry/geometry.hpp>
#include <simulator/particle.hpp>

namespace sim {
namespace geom {

/**
 * @brief Class that implements a sphere.
 */
class sphere : public geometry {
	private:
		/// Centre of the sphere.
		vec3 centre;
		/// Radius of the sphere.
		float R;

	public:
		/// Default constructor.
		sphere();
		/// Destructor.
		~sphere();

		// SETTERS

		void set_position(const vec3& p);

		// GETTERS

		bool is_inside(const vec3& p, float tol = 1.e-6f) const;
		geom_type get_geom_type() const;

		// OTHERS

		void update_upon_collision(particle *p) const;
		void display(ostream& os = cout) const;
};

} // -- namespace geom
} // -- namespace sim
