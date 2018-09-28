#pragma once

// glm includes
#include <glm/glm.hpp>
using namespace glm;

// Custom includes
#include "geometry.hpp"

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
		sphere();
		~sphere();

		void set_position(const vec3& p);
		bool is_inside(const vec3& p);

		geom_type get_geom_type() const;
};

} // -- namespace geom
} // -- namespace sim

