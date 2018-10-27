#include "render_geometry.hpp"

rgeom::rgeom() {
	render = true;
	color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

rgeom::~rgeom() { }

// SETTERS

void rgeom::set_render(bool r) { render = r; }
void rgeom::set_color(const vec4& c) { color = c; }

// GETTERS

bool rgeom::should_render() const { return render; }
const vec4& rgeom::get_color() const { return color; }
