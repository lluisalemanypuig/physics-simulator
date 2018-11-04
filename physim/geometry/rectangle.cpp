#include <physim/geometry/rectangle.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/math/math_private.hpp>

// LOCAL-DEFINED

namespace physim {
namespace geom {

// PRIVATE

// PUBLIC

rectangle::rectangle() : geometry() { }

rectangle::rectangle
(const math::vec3& p1,const math::vec3& p2,const math::vec3& p3,const math::vec3& p4)
	: geometry(), pl(plane(p1,p2,p3))
{
	__pm_assign_v(v1, p1);
	__pm_assign_v(v2, p2);
	__pm_assign_v(v3, p3);
	__pm_assign_v(v4, p4);

	// make sure that last vertex is on plane...
	assert(pl.is_inside(v4));

	__pm_min_vec4(min, v1,v2,v3,v4);
	__pm_max_vec4(max, v1,v2,v3,v4);
}

rectangle::rectangle(const rectangle& r) : geometry(r) {
	__pm_assign_v(v1, r.v1);
	__pm_assign_v(v2, r.v2);
	__pm_assign_v(v3, r.v3);
	__pm_assign_v(v4, r.v4);

	__pm_assign_v(min, r.min);
	__pm_assign_v(max, r.max);
}

rectangle::~rectangle() { }

// SETTERS

void rectangle::set_position(const math::vec3& v) {
	__pm_add_acc_v(v1, v);
	__pm_add_acc_v(v2, v);
	__pm_add_acc_v(v3, v);
	__pm_add_acc_v(v4, v);

	__pm_add_acc_v(min, v);
	__pm_add_acc_v(max, v);
}

// GETTERS

const plane& rectangle::get_plane() const {
	return pl;
}

bool rectangle::is_inside(const math::vec3& p, float tol) const {
	// if the point is not inside the plane,
	// for sure it is not inside the rectangle
	if (not pl.is_inside(p, tol)) {
		return false;
	}

	// If the point is on the associated plane
	// then it only remains to check that is is
	// inside the bounding box of the rectangle.
	// If so then the point is 'inside' the rectangle.

	if (not ((min.x <= p.x) and (p.x <= max.x))) {
		// outside x
		return false;
	}
	if (not ((min.y <= p.y) and (p.y <= max.y))) {
		// outside y
		return false;
	}
	if (not ((min.z <= p.z) and (p.z <= max.z))) {
		// outside z
		return false;
	}

	// inside box
	return true;
}

geom_type rectangle::get_geom_type() const {
	return geom_type::Rectangle;
}

bool rectangle::intersec_segment
(const math::vec3& p1, const math::vec3& p2) const
{
	math::vec3 intersection;
	return intersec_segment(p1, p2, intersection);
}

bool rectangle::intersec_segment
(const math::vec3& p1, const math::vec3& p2, math::vec3& p_inter) const
{
	// if the segment does not intersect the plane
	// surely it does not intersect the rectangle
	if (not pl.intersec_segment(p1,p2, p_inter)) {
		return false;
	}

	// intersection has been computed -> if this point
	// lies inside the rectangle then there is intersection
	if (is_inside(p_inter)) {
		return true;
	}
	return false;
}

// OTHERS

void rectangle::update_particle
(const math::vec3& pred_pos, const math::vec3& pred_vel, particles::free_particle *p) const
{
	pl.update_particle(pred_pos, pred_vel, p);
}

void rectangle::display(std::ostream& os) const {
	os << "I am a rectangle" << std::endl;
	os << "    with vertices:" << std::endl;
	os << "        - Point({" << v1.x << "," << v1.y << "," << v1.z << "})" << std::endl;
	os << "        - Point({" << v2.x << "," << v2.y << "," << v2.z << "})" << std::endl;
	os << "        - Point({" << v3.x << "," << v3.y << "," << v3.z << "})" << std::endl;
	os << "        - Point({" << v4.x << "," << v4.y << "," << v4.z << "})" << std::endl;
	os << "    and plane equation:" << std::endl;
	const math::vec3& n = pl.get_normal();
	os << "        " << n.x << "*x + " << n.y << "*y + " << n.z << "*z + "
	   << pl.get_constant() << " = 0" << std::endl;
}

} // -- namespace geom
} // -- namespace sim
