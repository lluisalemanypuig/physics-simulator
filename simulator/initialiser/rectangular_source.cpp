#include <simulator/initialiser/rectangular_source.hpp>

namespace physim {
namespace init {

// PROTECTED

void rect_source::make_pos_init() {
	pos = [this](particle *p) {
		const float rx = this->U01(this->E);
		const float rz = this->U01(this->E);
		p->set_previous_position(vec3(0.0f,0.0f,0.0f));
		p->set_position(
			vec3(
				this->x + rx*this->w,
				this->y,
				this->z + rz*this->h
			)
		);
	};
}

// PUBLIC

rect_source::rect_source() : initialiser() {
	random_device r;
	E = default_random_engine(r());
	U01 = uniform_real_distribution<float>(0.0f, 1.0f);
}

rect_source::rect_source(const rect_source& rs) : initialiser(rs) {
	E = rs.E;
	U01 = rs.U01;

	x = rs.x;
	y = rs.y;
	z = rs.z;
	w = rs.w;
	h = rs.h;

	// The function that initialises the position in 'rs'
	// has references to its E and U01 objects. If this
	// function was copied then the copied function will
	// keep the references to rs's E and U01 objects, not
	// to *this's. Therefore, remake the position initialser
	// function. Since its random...
	make_pos_init();
}

rect_source::~rect_source() { }

// SETTERS

void rect_source::set_rectangle_source(float _x,float _y,float _z, float _w,float _h) {
	x =_x; y = _y; z = _z; w = _w; h = _h;

	make_pos_init();
	make_vel_init();
}

} // -- namespace init
} // -- namespace physim
