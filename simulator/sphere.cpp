#include <simulator/sphere.hpp>

namespace sim {
namespace geom {

// PRIVATE

// PUBLIC

sphere::sphere() {
}

sphere::~sphere() {
}

void sphere::set_position(const vec3& p) {

}

bool sphere::is_inside(const vec3& p) {
	return false;
}

geom_type sphere::get_geom_type() const {
	return geom_type::sphere;
}

} // -- namespace geom
} // -- namespace sim
