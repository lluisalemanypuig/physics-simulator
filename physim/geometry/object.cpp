#include <physim/geometry/object.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <algorithm>
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

	cerr << "object::set_position (" << __LINE__
		 << ") - Update octree!" << endl;
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

		__pm3_min4(vmin, vmin, vs[i1], vs[i2], vs[i3]);
		__pm3_max4(vmax, vmax, vs[i1], vs[i2], vs[i3]);
	}

	octree.init(vs, trs);
}

// GETTERS

const structures::object_partition& object::get_partition() const {
	return octree;
}

geometry_type object::get_geom_type() const {
	return geometry_type::Object;
}

const std::vector<triangle>& object::get_triangles() const {
	return tris;
}

bool object::is_inside(const vec3& p, float tol) const {
	// check if point is inside bounding box
	if (not __pm3_inside_box(p, vmin,vmax)) {
		return false;
	}

	vector<size_t> idxs;
	octree.get_triangles(p, idxs);
	for (size_t t_idx : idxs) {
		if (tris[t_idx/3].is_inside(p, tol)) {
			return true;
		}
	}
	return false;
}

bool object::intersec_segment(const vec3& p1, const vec3& p2) const {
	vector<size_t> idxs;
	octree.get_triangles(p1, idxs);
	octree.get_triangles(p2, idxs);
	sort(idxs.begin(), idxs.end());
	auto last = unique(idxs.begin(), idxs.end());
	idxs.erase(last, idxs.end());
	for (size_t t_idx : idxs) {
		if (tris[t_idx/3].intersec_segment(p1,p2)) {
			return true;
		}
	}
	return false;
}

bool object::intersec_sphere(const vec3& c, float R) const {
	cerr << "object::intersec_sphere (" << __LINE__
		 << ") - To be implemented!" << endl;
	return false;
}

bool object::intersec_segment(const vec3& p1, const vec3& p2, vec3& p_inter) const {
	vector<size_t> idxs;
	octree.get_triangles(p1, idxs);
	octree.get_triangles(p2, idxs);
	sort(idxs.begin(), idxs.end());
	auto last = unique(idxs.begin(), idxs.end());
	idxs.erase(last, idxs.end());
	for (size_t t_idx : idxs) {
		if (tris[t_idx/3].intersec_segment(p1,p2,p_inter)) {
			return true;
		}
	}
	return false;
}

// OTHERS

void object::update_particle
(const vec3& pred_pos, const vec3& pred_vel, particles::free_particle *p)
const
{
	vector<size_t> idxs;
	octree.get_triangles(pred_pos, idxs);
	for (size_t t_idx : idxs) {
		if (tris[t_idx/3].intersec_segment(p->cur_pos, pred_pos)) {
			tris[t_idx/3].update_particle(pred_pos, pred_vel, p);
			return;
		}
	}
}

bool object::update_particle(
	const vec3& pred_pos, const vec3& pred_vel,
	const particles::free_particle *p, particles::free_particle *u
) const
{
	vector<size_t> idxs;
	octree.get_triangles(pred_pos, idxs);
	for (size_t t_idx : idxs) {
		if (tris[t_idx/3].intersec_segment(p->cur_pos, pred_pos)) {
			*u = *p;
			tris[t_idx/3].update_particle(pred_pos, pred_vel, u);
			return true;
		}
	}
	return false;
}

void object::update_particle(
	const vec3& pred_pos, const vec3& pred_vel,
	particles::sized_particle *p
) const
{
	cerr << "object::update_particle (" << __LINE__
		 << ") - To be implemented!" << endl;
}

bool object::update_particle(
	const math::vec3& pp, const math::vec3& pv,
	const particles::sized_particle *p, particles::sized_particle *u
) const
{
	cerr << "object::update_particle (" << __LINE__
		 << ") - To be implemented!" << endl;
}

void object::display() const {

}

} // -- namespace geom
} // -- namespace sim
