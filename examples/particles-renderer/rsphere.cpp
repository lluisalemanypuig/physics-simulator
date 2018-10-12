#include "render_geometry.hpp"

rsphere::rsphere() : rgeom() {
	s = nullptr;
}
rsphere::~rsphere() {}

physim::geom::geometry *rsphere::get_underlying() {
	return s;
}
