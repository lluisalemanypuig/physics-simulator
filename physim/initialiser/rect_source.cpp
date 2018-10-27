#include <physim/initialiser/rect_source.hpp>

#include <physim/math/math.hpp>

namespace physim {
namespace init {

// PROTECTED

void rect_source::make_pos_init() {
	pos = [this](particle *p) {
		const float l = this->U01(this->E);
		const float m = this->U01(this->E);

		/*p->set_position(
			this->S +
			l*this->w*this->u +	// span towards width
			m*this->h*this->v	// span towards height
		);*/

		__pm_add_vs_vs(p->get_position(), this->u,(l*this->w), this->v,(m*this->h));
		__pm_add_acc_v(p->get_position(), this->S);

		// copy the current position to the previous
		// position so that Verlet's solver works properly.
		p->save_position();
	};
}

// PUBLIC

rect_source::rect_source() : initialiser() {
	std::random_device r;
	E = std::default_random_engine(r());
	U01 = std::uniform_real_distribution<float>(0.0f, 1.0f);
}

rect_source::rect_source(const rect_source& rs) : initialiser(rs) {
	E = rs.E;
	U01 = rs.U01;

	/*S = rs.S;
	C = rs.C;

	u = rs.u;
	v = rs.v;
	n = rs.n;*/

	__pm_assign_v(S, rs.S);
	__pm_assign_v(C, rs.C);

	__pm_assign_v(u, rs.u);
	__pm_assign_v(v, rs.v);
	__pm_assign_v(n, rs.n);

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
(const math::vec3& _S, const math::vec3& _u,const math::vec3& _v, float _w,float _h)
{
	/*u = _u;
	v = _v;
	n = glm::normalize(glm::cross(u,v));*/

	__pm_assign_v(u, _u);
	__pm_assign_v(v, _v);
	__pm_cross(n,u,v);
	__pm_normalise(n,n);

	w = _w;
	h = _h;

	/*S = _S;
	C = S + 0.5f*w*u + 0.5f*h*v;*/

	__pm_assign_v(S, _S);
	__pm_assign_v(C, S);
	__pm_add_vs_vs(C, u,(0.5f*w), v,(0.5f*h));

	make_pos_init();
	make_vel_init();
}

void rect_source::set_straight_source(const math::vec3& _S, float _w,float _h) {
	/*u = vec3(1.0f, 0.0f, 0.0f);
	v = vec3(0.0f, 0.0f, 1.0f);
	n = vec3(0.0f, 1.0f, 0.0f);*/
	__pm_assign_c(u, 1.0f,0.0f,0.0f);
	__pm_assign_c(v, 0.0f,0.0f,1.0f);
	__pm_assign_c(n, 0.0f,1.0f,0.0f);

	w = _w;
	h = _h;

	/*S = _S;
	C = S + 0.5f*w*u + 0.5f*h*v;*/
	__pm_assign_v(S, _S);
	__pm_assign_v(C, S);
	__pm_add_vs_vs(C, u,(0.5f*w), v,(0.5f*h));

	make_pos_init();
	make_vel_init();
}

} // -- namespace init
} // -- namespace physim
