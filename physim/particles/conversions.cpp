#include <physim/particles/conversions.hpp>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace particles {

	void from_mesh_to_free(const mesh_particle& m, free_particle& f) {
		__pm3_assign_v(f.prev_pos, m.prev_pos);
		__pm3_assign_v(f.cur_pos, m.cur_pos);
		__pm3_assign_v(f.cur_vel, m.cur_vel);
		__pm3_assign_v(f.force, m.force);

		f.mass = m.mass;
		f.charge = m.charge;
		f.fixed = m.fixed;
		f.index = m.index;
	}

	void from_free_to_mesh(const free_particle& f, mesh_particle& m) {
		__pm3_assign_v(m.prev_pos, f.prev_pos);
		__pm3_assign_v(m.cur_pos, f.cur_pos);
		__pm3_assign_v(m.cur_vel, f.cur_vel);
		__pm3_assign_v(m.force, f.force);

		m.mass = f.mass;
		m.charge = f.charge;
		m.fixed = f.fixed;
		m.index = f.index;
	}

	void from_sized_to_agent(const sized_particle& s, agent_particle& a) {
		__pm3_assign_v(a.prev_pos, s.prev_pos);
		__pm3_assign_v(a.cur_pos, s.cur_pos);
		__pm3_assign_v(a.cur_vel, s.cur_vel);
		__pm3_assign_v(a.force, s.force);

		a.R = s.R;
	}

	void from_agent_to_sized(const agent_particle& a, sized_particle& s) {
		__pm3_assign_v(s.prev_pos, a.prev_pos);
		__pm3_assign_v(s.cur_pos, a.cur_pos);
		__pm3_assign_v(s.cur_vel, a.cur_vel);
		__pm3_assign_v(s.force, a.force);

		s.R = a.R;
	}

} // -- namespace particles
} // -- namespace physim
