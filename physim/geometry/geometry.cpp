#include <physim/geometry/geometry.hpp>

// C++ includes
#include <limits>
using namespace std;

// physim includes
#include <physim/math/private/math3/base.hpp>

namespace physim {
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

geometry_type geometry::get_geom_type() const {
	return geometry_type::none;
}

} // -- namespace geom
} // -- namespace sim
