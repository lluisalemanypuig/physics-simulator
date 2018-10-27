#include "render_geometry.hpp"

rrectangle::rrectangle() : rgeom() {
	rl = nullptr;
}
rrectangle::~rrectangle() {}

physim::geom::geometry *rrectangle::get_underlying() {
	return rl;
}
