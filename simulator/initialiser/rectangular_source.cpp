#include <simulator/initialiser/rectangular_source.hpp>

namespace physim {
namespace init {

// PROTECTED

void rectangular_source::make_pos_init() {
	pos = [this](particle *p) {
		float r = this->U01(this->E);
		p->set_previous_position(vec3(0.0f,0.0f,0.0f));
		p->set_position(
			vec3(
				this->x + r*this->w,
				this->y,
				this->z + r*this->h
			)
		);
	};
}

// PUBLIC

rectangular_source::rectangular_source() : initialiser() {
	random_device r;
	E = default_random_engine(r());
	U01 = uniform_real_distribution<float>(0.0f, 1.0f);
}

rectangular_source::~rectangular_source() { }

// SETTERS

void rectangular_source::set_rectangle_source(float _x,float _y,float _z, float _w,float _h) {
	x =_x; y = _y; z = _z; w = _w; h = _h;

	make_pos_init();
	make_vel_init();
}

} // -- namespace init
} // -- namespace physim
