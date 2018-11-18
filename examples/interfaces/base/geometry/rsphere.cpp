#include <base/geometry/rsphere.hpp>

// base includes
#include <base/include_gl.hpp>

// C++ includes
#include <iostream>
using namespace std;

rsphere::rsphere() : rgeom() {
	t = rendered_geometry_type::sphere;
}
rsphere::~rsphere() {}

// SETTERS

void rsphere::set_center(const vec3& _c) {
	C = _c;
}

void rsphere::set_radius(float _r) {
	R = _r;
}

// GETETRS

const vec3& rsphere::center() const {
	return C;
}

float rsphere::radius() const {
	return R;
}

// OTHERS

void rsphere::translate_object() const {
	glTranslatef(C.x,C.y,C.z);
	glScalef(R*2.0f,R*2.0f,R*2.0f);
}

void rsphere::draw_geometry() const {
	cout << "rsphere::draw_geometry() - Error!" << endl;
	cout << "    This object does not have a model and" << endl;
	cout << "    I don't know how to draw a sphere" << endl;
	cout << "    using glBegin(), glEnd()" << endl;
}

void rsphere::make_box(box& b) const {
	if (_model != nullptr) {
		b.set_min_max(C - vec3(R,R,R), C + vec3(R,R,R));
	}
	else {
		_model->make_box(b);
	}
}
