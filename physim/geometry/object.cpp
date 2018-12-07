#include <physim/geometry/object.hpp>

// C++ includes
#include <iostream>
using namespace std;

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace geom {

// PRIVATE

// PUBLIC

object::object() : geometry() {

}

object::object(const object& o) : geometry(o) {
	verts = o.verts;
	tris = o.tris;
}

object::~object() { }

// SETTERS

void object::set_position(const math::vec3& dir) {
	for (math::vec3& v : verts) {
		__pm3_add_acc_v(v, dir);
	}
}

void object::set_vertices(const std::vector<math::vec3>& vs) {
	verts = vs;
}

void object::set_triangles(const std::vector<size_t>& trs) {
	tris = trs;
}

// GETTERS

geom_type object::get_geom_type() const {
	return geom_type::Object;
}

bool object::is_inside(const math::vec3& p, float tol) const {
	return false;
}

bool object::intersec_segment(const math::vec3& p1, const math::vec3& p2) const {
	return false;
}

bool object::intersec_sphere(const math::vec3& c, float R) const {
	return false;
}

bool object::intersec_segment(const math::vec3& p1, const math::vec3& p2, math::vec3& p_inter) const {
	return false;
}

// OTHERS

void object::update_particle
(const math::vec3& pred_pos, const math::vec3& pred_vel, particles::free_particle *p)
const
{

}

void object::correct_position(
	const math::vec3& pred_pos, const particles::sized_particle *p,
	math::vec3& correct_position
) const
{

}

void object::update_particle
(const math::vec3& pred_pos, const math::vec3& pred_vel, particles::sized_particle *p)
const
{

}

void object::display(std::ostream& os) const {

}

} // -- namespace geom
} // -- namespace sim
