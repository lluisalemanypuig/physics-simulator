#include <simulator/geometry/geometry.hpp>

namespace physim {
namespace geom {

// PUBLIC

geometry::geometry() { }
geometry::geometry(const geometry& ) { }
geometry::~geometry() { }

geom_type geometry::get_geom_type() const {
	return geom_type::none;
}

} // -- namespace geom
} // -- namespace sim
