#include <render/geometry/robject.hpp>

// C++ includes
#include <iostream>
using namespace std;

// glm inlcudes
#include <glm/gtc/matrix_transform.hpp>

// render includes
#include <render/include_gl.hpp>

robject::robject() : rgeom() {
	t = rendered_geometry_type::object;
}
robject::~robject() {}

// SETTERS

// GETETRS

// OTHERS

void robject::draw_geometry() const {
	cerr << "robject::draw_geometry() - Error!" << endl;
	cerr << "    This object does not have a mesh and" << endl;
	cerr << "    I don't know how to draw this" << endl;
	cerr << "    using glBegin(), glEnd()" << endl;
}

void robject::make_model_matrix(glm::mat4&) const {
	// do nothing
}

void robject::make_box(box& b) const {
	if (_model != nullptr) {
		_model->make_box(b);
	}
	else {
		cerr << "robject::make_box() - Error!" << endl;
		cerr << "    Do not know how to make a box out of a" << endl;
		cerr << "    generic geometric object without model." << endl;
	}
}
