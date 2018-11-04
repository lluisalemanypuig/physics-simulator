#include <physim/particles/conversions.hpp>

// physim includes
#include <physim/math/math_private.hpp>

namespace physim {
namespace particles {

	void from_mesh_to_free(const mesh_particle& m, free_particle& f) {
		__pm_assign_v(f.prev_pos, m.prev_pos);
		__pm_assign_v(f.cur_pos, m.cur_pos);
		__pm_assign_v(f.cur_vel, m.cur_vel);
		__pm_assign_v(f.force, m.force);

		f.mass = m.mass;
		f.charge = m.charge;
		f.fixed = m.fixed;
		f.index = m.index;
	}

	void from_free_to_mesh(const free_particle& f, mesh_particle& m) {
		__pm_assign_v(m.prev_pos, f.prev_pos);
		__pm_assign_v(m.cur_pos, f.cur_pos);
		__pm_assign_v(m.cur_vel, f.cur_vel);
		__pm_assign_v(m.force, f.force);

		m.mass = f.mass;
		m.charge = f.charge;
		m.fixed = f.fixed;
		m.index = f.index;
	}

} // -- namespace particles
} // -- namespace physim
