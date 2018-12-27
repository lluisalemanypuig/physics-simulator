#include <physim/geometry/geometry.hpp>

// C++ includes
#include <limits>
using namespace std;

// physim includes
#include <physim/math/private/math3/base.hpp>

namespace physim {
using namespace math;

namespace geometric {

// PUBLIC

geometry::geometry() {
	__pm3_assign_s(vmin,  numeric_limits<float>::max());
	__pm3_assign_s(vmax, -numeric_limits<float>::max());
}
geometry::geometry(const geometry& g) {
	__pm3_assign_v(vmin, g.vmin);
	__pm3_assign_v(vmax, g.vmax);
}
geometry::~geometry() { }

// GETTERS

geometry_type geometry::get_geom_type() const {
	return geometry_type::none;
}

const vec3& geometry::get_min() const {
	return vmin;
}
const vec3& geometry::get_max() const {
	return vmax;
}

vec3 geometry::get_box_center() const {
	return (vmin + vmax)/2;
}

} // -- namespace geometric
} // -- namespace physim
