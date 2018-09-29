#include <simulator/geometry/triangle.hpp>

namespace sim {
namespace geom {

// PRIVATE

// PUBLIC

triangle::triangle() {
}

triangle::~triangle() {
}

void triangle::set_position(const vec3& p) {

}

bool triangle::is_inside(const vec3& p, float tol) {
	return false;
}

geom_type triangle::get_geom_type() const {
	return geom_type::Triangle;
}

} // -- namespace geom
} // -- namespace sim
