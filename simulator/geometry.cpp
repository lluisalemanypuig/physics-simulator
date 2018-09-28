#include <simulator/geometry.hpp>

namespace sim {
namespace geom {

// PUBLIC

geometry::geometry() { }
geometry::~geometry() { }

geom_type geometry::get_geom_type() const {
	return geom_type::none;
}

} // -- namespace geom
} // -- namespace sim
