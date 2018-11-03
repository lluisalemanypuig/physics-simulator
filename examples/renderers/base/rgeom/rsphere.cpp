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
}

void rsphere::draw_geometry() const {
	cout << "rsphere::draw_geometry() - Error!" << endl;
	cout << "    This object does not have a model and" << endl;
	cout << "    I don't know how to draw a sphere" << endl;
	cout << "    using glBegin(), glEnd()" << endl;
}

