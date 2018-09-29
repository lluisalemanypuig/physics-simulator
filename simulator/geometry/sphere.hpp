#pragma once

// glm includes
#include <glm/glm.hpp>
using namespace glm;

// Custom includes
#include <simulator/geometry/geometry.hpp>

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

		bool is_inside(const vec3& p, float tol = 1.e-7f);
		geom_type get_geom_type() const;
};

} // -- namespace geom
} // -- namespace sim
