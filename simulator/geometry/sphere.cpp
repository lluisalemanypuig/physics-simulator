#include <simulator/geometry/sphere.hpp>

namespace physim {
namespace geom {

// PRIVATE

// PUBLIC

sphere::sphere() { }

sphere::sphere(const sphere& s) : geometry(s) {
	centre = s.centre;
	R = s.R;
}

sphere::~sphere() { }

void sphere::set_position(const vec3& p) {

}

bool sphere::is_inside(const vec3& p, float tol) const {
	return false;
}

geom_type sphere::get_geom_type() const {
	return geom_type::Sphere;
}

// OTHERS

void sphere::update_upon_collision(particle *p) const {

}

void sphere::display(ostream& os) const {

}

} // -- namespace geom
} // -- namespace sim
