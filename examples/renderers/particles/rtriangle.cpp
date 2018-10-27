#include "render_geometry.hpp"

rtriangle::rtriangle() : rgeom() {
	tl = nullptr;
}
rtriangle::~rtriangle() {}

physim::geom::geometry *rtriangle::get_underlying() {
	return tl;
}
