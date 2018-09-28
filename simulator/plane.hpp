#pragma once

// glm includes
#include <glm/glm.hpp>
using namespace glm;

// Custom includes
#include "geometry.hpp"

namespace sim {
namespace geom {

/**
 * @brief Class that implements a plane.
 */
class plane : public geometry {
	private:
		/// Normal of the plane.
		vec3 normal;
		/// Constant, independent term of the plane equation.
		float dconst;

	public:
		plane();
		~plane();

		void set_position(const vec3& p);
		bool is_inside(const vec3& p);

		geom_type get_geom_type() const;
};

} // -- namespace geom
} // -- namespace sim

