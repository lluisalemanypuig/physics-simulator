#include <physim/emitter/free_emitters/rect_source.hpp>

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/math/private/math3.hpp>

namespace physim {
using namespace particles;
using namespace math;

namespace emitters {
namespace free_emitters {

// PROTECTED

void rect_source::make_pos_init() {
	pos = [this](base_particle& p) {
		const float l = this->U01(this->E);
		const float m = this->U01(this->E);

		__pm3_add_vs_vs_v(p.cur_pos, this->u,(l*this->w), this->v,(m*this->h), this->S);

		// copy the current position to the previous
		// position so that Verlet's solver works properly.
		p.save_position();
	};
}

// PUBLIC

rect_source::rect_source() : free_emitter() {
	std::random_device r;
	E = std::default_random_engine(r());
	U01 = std::uniform_real_distribution<float>(0.0f, 1.0f);
}

rect_source::rect_source(const rect_source& rs) : free_emitter(rs) {
	E = rs.E;
	U01 = rs.U01;

	__pm3_assign_v(S, rs.S);
	__pm3_assign_v(C, rs.C);

	__pm3_assign_v(u, rs.u);
	__pm3_assign_v(v, rs.v);
	__pm3_assign_v(n, rs.n);

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

void rect_source::set_rectangular_source
(const vec3& _S, const vec3& _u,const vec3& _v, float _w,float _h)
{
	__pm3_assign_v(u, _u);
	__pm3_assign_v(v, _v);
	__pm3_cross(n,u,v);
	__pm3_normalise(n,n);

	w = _w;
	h = _h;

	__pm3_assign_v(S, _S);
	__pm3_assign_v(C, S);
	__pm3_add_vs_vs(C, u,(0.5f*w), v,(0.5f*h));

	make_pos_init();
	make_vel_init();
}

void rect_source::set_straight_source(const vec3& _S, float _w,float _h) {
	__pm3_assign_c(u, 1.0f,0.0f,0.0f);
	__pm3_assign_c(v, 0.0f,0.0f,1.0f);
	__pm3_assign_c(n, 0.0f,1.0f,0.0f);

	w = _w;
	h = _h;

	__pm3_assign_v(S, _S);
	__pm3_assign_v(C, S);
	__pm3_add_vs_vs(C, u,(0.5f*w), v,(0.5f*h));

	make_pos_init();
	make_vel_init();
}

} // -- namespace free_emitters
} // -- namespace emitters
} // -- namespace physim
