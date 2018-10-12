#include "render_geometry.hpp"

rplane::rplane() : rgeom() {
	pl = nullptr;
}
rplane::~rplane() {}

physim::geom::geometry *rplane::get_underlying() {
	return pl;
}
