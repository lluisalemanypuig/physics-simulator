#include <physim/geometry/object.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <iostream>
using namespace std;

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
using namespace math;

namespace geometry {

// PRIVATE

// PUBLIC

object::object() : geometry() {

}

object::object(const object& o) : geometry(o) {
	tris = o.tris;
	octree.copy(o.octree);
}

object::~object() { }

// SETTERS

void object::set_position(const vec3& dir) {
	for (triangle& t : tris) {
		t.set_position(dir);
	}

	cerr << "object::set_position (" << __LINE__ << ") Update octree!" << endl;
}

void object::set_triangles
(const std::vector<vec3>& vs, const std::vector<size_t>& trs)
{
	assert(trs.size()%3 == 0);

	tris.resize(trs.size()/3);
	for (size_t i = 0; i < tris.size(); ++i) {
		size_t i1 = trs[3*i    ];
		size_t i2 = trs[3*i + 1];
		size_t i3 = trs[3*i + 2];
		tris[i] = triangle(vs[i1], vs[i2], vs[i3]);
	}

	octree.init(vs, trs);
}

// GETTERS

geometry_type object::get_geom_type() const {
	return geometry_type::Object;
}

const std::vector<triangle>& object::get_triangles() const {
	return tris;
}

bool object::is_inside(const vec3& p, float tol) const {
	return false;
}

bool object::intersec_segment(const vec3& p1, const vec3& p2) const {
	return false;
}

bool object::intersec_sphere(const vec3& c, float R) const {
	return false;
}

bool object::intersec_segment(const vec3& p1, const vec3& p2, vec3& p_inter) const {
	return false;
}

// OTHERS

void object::update_particle
(const vec3& pred_pos, const vec3& pred_vel, particles::free_particle *p)
const
{

}

void object::correct_position(
	const vec3& pred_pos, const particles::sized_particle *p,
	vec3& correct_position
) const
{

}

void object::update_particle
(const vec3& pred_pos, const vec3& pred_vel, particles::sized_particle *p)
const
{

}

void object::display() const {

}

} // -- namespace geom
} // -- namespace sim
