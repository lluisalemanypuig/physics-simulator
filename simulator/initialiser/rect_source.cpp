#include <simulator/initialiser/rect_source.hpp>

namespace physim {
namespace init {

// PROTECTED

void rect_source::make_pos_init() {
	pos = [this](particle *p) {
		const float l = this->U01(this->E);
		const float m = this->U01(this->E);
		p->set_position(
			this->S +
			l*this->w*this->u +	// span towards width
			m*this->h*this->v	// span towards height
		);
		// copy the current position to the previous
		// position so that Verlet's solver works properly.
		p->save_position();
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

	S = rs.S;
	C = rs.C;

	u = rs.u;
	v = rs.v;
	n = rs.n;

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

void rect_source::set_rectangular_source(const vec3& _S, const vec3& _u,const vec3& _v, float _w,float _h) {
	u = _u;
	v = _v;
	n = glm::normalize(glm::cross(u,v));

	w = _w;
	h = _h;

	S = _S;
	C = S + 0.5f*w*u + 0.5f*h*v;

	make_pos_init();
	make_vel_init();
}

void rect_source::set_straight_source(const vec3& _S, float _w,float _h) {
	u = vec3(1.0f, 0.0f, 0.0f);
	v = vec3(0.0f, 0.0f, 1.0f);
	n = vec3(0.0f, 1.0f, 0.0f);

	w = _w;
	h = _h;

	S = _S;
	C = S + 0.5f*w*u + 0.5f*h*v;

	make_pos_init();
	make_vel_init();
}

} // -- namespace init
} // -- namespace physim
