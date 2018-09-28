#include <simulator/triangle.hpp>

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

bool triangle::is_inside(const vec3& p) {
	return false;
}

geom_type triangle::get_geom_type() const {
	return geom_type::triangle;
}

} // -- namespace geom
} // -- namespace sim
