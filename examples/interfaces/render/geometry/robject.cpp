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

void robject::set_boxes(const physim_boxes_set& pbs) {
	for (const pair<physim::math::vec3,physim::math::vec3>& p : pbs) {
		glm::vec3 vmin, vmax;
		vmin.x = p.first.x;
		vmin.y = p.first.y;
		vmin.z = p.first.z;

		vmax.x = p.second.x;
		vmax.y = p.second.y;
		vmax.z = p.second.z;

		box B;
		B.set_min_max(vmin, vmax);

		boxes.push_back(B);
	}
}

// GETTERS

const vector<box>& robject::get_boxes() const {
	return boxes;
}

// OTHERS

void robject::make_boxes_buffers() {
	for (box& b : boxes) {
		b.make_buffers();
	}
}

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
