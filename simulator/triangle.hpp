#pragma once

// glm includes
#include <glm/glm.hpp>
using namespace glm;

// Custom includes
#include "geometry.hpp"

namespace sim {
namespace geom {

/**
 * @brief Class that implements a triangle.
 */
class triangle : public geometry {
	private:
		/// The vertices of the triangle.
		vec3 vertex1, vertex2, vertex3;

	public:
		triangle();
		~triangle();

		void set_position(const vec3& p);
		bool is_inside(const vec3& p);

		geom_type get_geom_type() const;
};

} // -- namespace geom
} // -- namespace sim



