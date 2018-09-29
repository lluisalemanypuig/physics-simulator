#pragma once

// glm includes
#include <glm/glm.hpp>
using namespace glm;

// Custom includes
#include <simulator/geometry/geometry.hpp>

namespace sim {
namespace geom {

/**
 * @brief Class that implements a triangle.
 */
class triangle : public geometry {
	private:
		/// The first vertex of the triangle.
		vec3 v1;
		/// The second vertex of the triangle.
		vec3 v2;
		/// The third vertex of the triangle.
		vec3 v3;

	public:
		/// Default constructor.
		triangle();
		/// Destructor.
		~triangle();

		// SETTERS

		void set_position(const vec3& p);

		// GETTERS

		bool is_inside(const vec3& p, float tol = 1.e-7f) const;
		geom_type get_geom_type() const;
};

} // -- namespace geom
} // -- namespace sim
