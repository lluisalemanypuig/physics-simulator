#include <base/rgeom/rendered_geometry.hpp>

// base includes
#include <base/include_gl.hpp>

// C++ includes
#include <iostream>
using namespace std;

rsphere::rsphere() : rgeom() {
	t = rendered_geometry_type::sphere;
}
rsphere::~rsphere() {}

// OTHERS

void rsphere::translate_object() const {
	glTranslatef(c.x,c.y,c.z);
	glScalef(r*2.0f,r*2.0f,r*2.0f);
}

void rsphere::draw_geometry() const {
	cout << "rsphere::draw_geometry() - Error!" << endl;
	cout << "    This object does not have a model and" << endl;
	cout << "    I don't know how to draw a sphere" << endl;
	cout << "    using glBegin(), glEnd()" << endl;
}

void rsphere::make_box(box& b) const {
	if (model != nullptr) {
		b.set_min_max(c - vec3(r,r,r), c + vec3(r,r,r));
	}
	else {
		model->make_box(b);
	}
}
