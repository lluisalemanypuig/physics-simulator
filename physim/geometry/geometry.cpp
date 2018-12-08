#include <physim/geometry/geometry.hpp>

namespace physim {
namespace geometry {

// PUBLIC

geometry::geometry() { }
geometry::geometry(const geometry& ) { }
geometry::~geometry() { }

geometry_type geometry::get_geom_type() const {
	return geometry_type::none;
}

} // -- namespace geom
} // -- namespace sim
