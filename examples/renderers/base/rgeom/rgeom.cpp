#include <base/rgeom/rendered_geometry.hpp>

#include <base/include_gl.hpp>

rgeom::rgeom() {
	render = true;
	r = 1.0f;
	g = 0.0f;
	b = 0.0f;
	a = 1.0f;

	t = rendered_geometry_type::none;
	model = nullptr;
}

rgeom::~rgeom() {
	if (model != nullptr) {
		delete model;
		model = nullptr;
	}
}

// SETTERS

void rgeom::set_render(bool r) { render = r; }
void rgeom::set_color(float _r, float _g, float _b, float _a) {
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}
void rgeom::set_model(rendered_model *_m) {
	model = _m;
}

// GETTERS

bool rgeom::should_render() const { return render; }

rendered_geometry_type rgeom::get_type() const {
	return t;
}

rendered_model *rgeom::get_model() {
	return model;
}

// OTHERS

void rgeom::translate_object() const {
	// method left blank since this is abstract geometry
}

void rgeom::draw() const {
	if (model != nullptr) {
		glEnable(GL_LIGHTING);
		glPushMatrix();
			translate_object();
			model->render();
		glPopMatrix();
	}
	else {
		draw_geometry();
	}
}
