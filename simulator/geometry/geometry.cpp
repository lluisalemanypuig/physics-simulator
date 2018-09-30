#include <simulator/geometry/geometry.hpp>

#include <iostream>
using namespace std;

namespace sim {
namespace geom {

// PUBLIC

geometry::geometry() { }
geometry::~geometry() { }

void geometry::display() const {
	cout << "no geometry" << endl;
}

geom_type geometry::get_geom_type() const {
	return geom_type::none;
}

} // -- namespace geom
} // -- namespace sim
