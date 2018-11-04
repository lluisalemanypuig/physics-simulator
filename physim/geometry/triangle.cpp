#include <physim/geometry/triangle.hpp>

// physim includes
#include <physim/math/math_private.hpp>

inline float triangle_area
(const physim::math::vec3 p1, const physim::math::vec3 p2, const physim::math::vec3 p3)
{
	physim::math::vec3 C;
	__pm_cross_diff(C, p1,p2,p3);
	return __pm_norm(C)/2.0;
}

namespace physim {
namespace geom {

// LOCAL-DEFINED

// PRIVATE

// PUBLIC

triangle::triangle() : geometry() { }

triangle::triangle
(const math::vec3& p1,const math::vec3& p2,const math::vec3& p3)
	: geometry(), pl(plane(p1,p2,p3))
{
	__pm_assign_v(v1, p1);
	__pm_assign_v(v2, p2);
	__pm_assign_v(v3, p3);
}

triangle::triangle(const triangle& t)
	: geometry(t), pl(t.pl)
{
	__pm_assign_v(v1, t.v1);
	__pm_assign_v(v2, t.v2);
	__pm_assign_v(v3, t.v3);
}

triangle::~triangle() { }

// SETTERS

void triangle::set_position(const math::vec3& v) {
	__pm_add_acc_v(v1, v);
	__pm_add_acc_v(v2, v);
	__pm_add_acc_v(v3, v);
	pl.set_position(v1);
}

// GETTERS

const plane& triangle::get_plane() const {
	return pl;
}

bool triangle::is_inside(const math::vec3& p, float tol) const {
	// if the point is not inside the plane,
	// for sure it is not inside the triangle
	if (not pl.is_inside(p, tol)) {
		return false;
	}

	// compute areas of the triangles
	float a1 = triangle_area(p,  v2, v3);
	float a2 = triangle_area(v1,  p, v3);
	float a3 = triangle_area(v1, v2,  p);
	float A  = triangle_area(v1, v2, v3);

	// test inside/outside
	if ((a1 + a2 + a3 - A) <= tol) {
		return true;
	}
	return false;
}

geom_type triangle::get_geom_type() const {
	return geom_type::Triangle;
}

bool triangle::intersec_segment(const math::vec3& p1, const math::vec3& p2) const {
	math::vec3 intersection;
	return intersec_segment(p1, p2, intersection);
}

bool triangle::intersec_segment
(const math::vec3& p1, const math::vec3& p2, math::vec3& p_inter) const
{
	// if the segment does not intersect the plane
	// surely it does not intersect the triangle
	if (not pl.intersec_segment(p1,p2, p_inter)) {
		return false;
	}

	// intersection has been computed -> if this point
	// lies inside the triangle then there is intersection
	if (is_inside(p_inter)) {
		return true;
	}
	return false;
}

// OTHERS

void triangle::update_particle
(const math::vec3& pred_pos, const math::vec3& pred_vel, particles::free_particle *p) const
{
	pl.update_particle(pred_pos, pred_vel, p);
}

void triangle::display(std::ostream& os) const {
	os << "I am a triangle" << std::endl;
	os << "    with vertices:" << std::endl;
	os << "        - Point({" << v1.x << "," << v1.y << "," << v1.z << "})" << std::endl;
	os << "        - Point({" << v2.x << "," << v2.y << "," << v2.z << "})" << std::endl;
	os << "        - Point({" << v3.x << "," << v3.y << "," << v3.z << "})" << std::endl;
	os << "    and plane equation:" << std::endl;
	const math::vec3& n = pl.get_normal();
	os << "        " << n.x << "*x + " << n.y << "*y + " << n.z << "*z + "
	   << pl.get_constant() << " = 0" << std::endl;
}

} // -- namespace geom
} // -- namespace sim
