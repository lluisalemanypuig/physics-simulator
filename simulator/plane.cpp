#include <simulator/plane.hpp>

namespace sim {
namespace geom {

// PRIVATE

// PUBLIC

plane::plane() {
	normal = vec3(0.0f,0.0f,0.0f);
	dconst = 0.0f;
}

plane::plane(const vec3& p, const vec3& n) {
	normal = glm::normalize(n);
	dconst = -glm::dot(p, normal);
}

plane::plane(const vec3& p0, const vec3& p1, const vec3& p2) {
	vec3 v1 = p1 - p0;
	vec3 v2 = p2 - p0;
	normal = glm::normalize(glm::cross(v1, v2));
	dconst = -glm::dot(p0, normal);
}

plane::~plane() { }

void plane::set_position(const vec3& p) {
	dconst = -glm::dot(p, normal);
}

bool plane::is_inside(const vec3& p, float tol) const {
	float dist = glm::dot(p, normal) + dconst;
	if (dist <= tol) {
		return true;
	}
	return false;
}

geom_type plane::get_geom_type() const {
	return geom_type::plane;
}

float plane::dist_point_plane(const vec3& p) const {
	return glm::dot(p, normal) + dconst;
}

vec3 plane::closest_point_plane(const vec3& p) const {
	float r = (-dconst - glm::dot(p, normal));
	return p + r*normal;
}

bool plane::intersec_segment(const vec3& p1, const vec3& p2, vec3& p_inter) const {
	float d1 = dist_point_plane(p1);
	float d2 = dist_point_plane(p2);
	if (d1*d2 > 0) {
		return false;
	}

	float r = (-dconst - glm::dot(p1, normal)) / glm::dot((p2 - p1), normal);
	p_inter = (1 - r)*p1 + r*p2;
	return true;
}

} // -- namespace geom
} // -- namespace sim
