#include "rendered_geometry.hpp"

rgeom::rgeom() {
	render = true;
	r = 1.0f;
	g = 0.0f;
	b = 0.0f;
	a = 1.0f;

	t = rendered_geometry_type::none;
}

rgeom::~rgeom() { }

// SETTERS

void rgeom::set_render(bool r) { render = r; }
void rgeom::set_color(float _r, float _g, float _b, float _a) {
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

// GETTERS

bool rgeom::should_render() const { return render; }

rendered_geometry_type rgeom::get_type() const {
	return t;
}
