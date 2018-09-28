#include "plane.hpp"

namespace sim {
namespace geom {

// PRIVATE

// PUBLIC

plane::plane() {
}

plane::~plane() {
}

void plane::set_position(const vec3& p) {

}

bool plane::is_inside(const vec3& p) {
	return false;
}

geom_type plane::get_geom_type() const {
	return geom_type::plane;
}

} // -- namespace geom
} // -- namespace sim
