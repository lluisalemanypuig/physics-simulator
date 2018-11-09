#include <physim/meshes/mesh2d_regular.hpp>

// C includes
#include <stdlib.h>
#include <assert.h>

// physim includes
#include <physim/math/math_private.hpp>

namespace physim {
namespace meshes {

// variable C is a class member
#define idx(i,j) ((i)*C + (j))

// this macro requires some local variables to be defined:
//     math::vec3 F1_m1;
//     math::vec3 dir;
//     math::vec3 dvel;
//     float elastic_term, damping_term, dist;
// Other values are class members:
//     Ke, Kd
/*
	// direction vector
	__pm_sub_v_v(dir, ps[i + 1]->cur_pos, ps[i]->cur_pos);
	dist = __pm_norm(dir);
	__pm_normalise(dir, dir);
	// difference of velocities
	__pm_sub_v_v(dvel, ps[i + 1]->cur_vel, ps[i]->cur_vel);
	// compute terms
	elastic_term = Ke*(dist - ds[i]);
	damping_term = Kd*__pm_dot(dvel, dir);
	// compute forces
	__pm_assign_vs(F1_m1, dir, elastic_term + damping_term);
	__pm_add_acc_v(ps[i]->force, F1_m1);
	__pm_invert(F1_m1, F1_m1);
	__pm_add_acc_v(ps[i + 1]->force, F1_m1);
*/
// d: original distance between particles i and j
#define compute_forces(i,j, d)											\
	__pm_sub_v_v(dir, ps[j]->cur_pos, ps[i]->cur_pos);					\
	dist = __pm_norm(dir);												\
	__pm_normalise(dir, dir);											\
	__pm_sub_v_v(dvel, ps[j]->cur_vel, ps[i]->cur_vel);					\
	__pm_assign_vs(F1_m1, dir, Ke*(dist - d) + Kd*__pm_dot(dvel, dir));	\
	__pm_add_acc_v(ps[i]->force, F1_m1);								\
	__pm_invert(F1_m1, F1_m1);											\
	__pm_add_acc_v(ps[j]->force, F1_m1)

// PUBLIC

mesh2d_regular::mesh2d_regular() : mesh() {
	mt = mesh_type::d2_regular;
	sb_ds = nullptr;
	R = 0;
	C = 0;
}

mesh2d_regular::mesh2d_regular(float ke, float kd) : mesh(ke,kd) {
	mt = mesh_type::d2_regular;
	sb_ds = nullptr;
	R = 0;
	C = 0;
}

mesh2d_regular::~mesh2d_regular() {
	clear();
}

// MODIFIERS

void mesh2d_regular::make_initial_state() {
	if (sb_ds != nullptr) {
		free(sb_ds);
	}
	sb_ds = (math::vec6 *)malloc(R*(C - 1)*sizeof(math::vec6));

	/*
	 * Since this is done only once, 'if' statements
	 * can be tolerated here (I'm talking about the 'if (i < R - 2 ...)').
	 * In the force update, the 'if' statements can't really be avoided
	 * without doing the same loops more than twice.
	 */

	for (size_t i = 0; i < C - 1; ++i) {
		for (size_t j = 0; j < R - 1; ++j) {

			// stretch forces
			sb_ds[idx(i,j)].x = __pm_dist(ps[idx(i,j)]->cur_pos, ps[idx(i+1,j)]->cur_pos);
			sb_ds[idx(i,j)].y = __pm_dist(ps[idx(i,j)]->cur_pos, ps[idx(i,j+1)]->cur_pos);

			// bend forces
			if (i < R - 2 and j < C - 2) {
				sb_ds[idx(i,j)].z = __pm_dist(ps[idx(i,j)]->cur_pos, ps[idx(i+2,j)]->cur_pos);
				sb_ds[idx(i,j)].u = __pm_dist(ps[idx(i,j)]->cur_pos, ps[idx(i,j+2)]->cur_pos);
			}

			// shear forces
			if (i > 0) {
				sb_ds[idx(i,j)].v = __pm_dist(ps[idx(i,j)]->cur_pos, ps[idx(i-1,j+1)]->cur_pos);
			}
			if (i < R - 1) {
				sb_ds[idx(i,j)].w = __pm_dist(ps[idx(i,j)]->cur_pos, ps[idx(i+1,j+1)]->cur_pos);
			}
		}
	}
}

void mesh2d_regular::update_forces() {
	assert(sb_ds != nullptr);

	math::vec3 F1_m1;
	math::vec3 dir;
	math::vec3 dvel;
	float dist;

	// -------
	// -- 1 --
	// -------

	// all stretch, bend and shear forces for [1,R-3]x[0,C-3]
	for (size_t i = 0; i < R - 1; ++i) {
		for (size_t j = 0; j < C - 1; ++j) {

			if (stretch) {
				if (i < R - 1) {
					compute_forces( idx(i,j), idx(i + 1,j), sb_ds[idx(i,j)].x );
				}
				if (j < C - 1) {
					compute_forces( idx(i,j), idx(i,j + 1), sb_ds[idx(i,j)].y );
				}
			}

			if (bend) {
				if (i + 2 < R) {
					compute_forces( idx(i,j), idx(i + 2,j), sb_ds[idx(i,j)].z );
				}
				if (j + 2 < C) {
					compute_forces( idx(i,j), idx(i,j + 2), sb_ds[idx(i,j)].u );
				}
			}

			if (shear) {
				if (j + 1 < C - 1) {
					if (i > 0) {
						compute_forces( idx(i,j), idx(i - 1,j + 1), sb_ds[idx(i,j)].v );
					}
					if (i < R - 1) {
						compute_forces( idx(i,j), idx(i + 1,j + 1), sb_ds[idx(i,j)].w );
					}
				}
			}
		}
	}

}

void mesh2d_regular::clear() {
	mesh::clear();

	if (sb_ds != nullptr) {
		free(sb_ds);
		sb_ds = nullptr;
	}
}

// SETTERS

void mesh2d_regular::set_dimensions(size_t r, size_t c) {
	assert(r*c == N);
	assert(r >= 2);
	assert(c >= 2);

	R = r;
	C = c;
}

// GETTERS

void mesh2d_regular::get_dimensions(size_t& r, size_t& c) {
	r = R;
	c = C;
}

size_t mesh2d_regular::get_global_index(size_t i, size_t j) const {
	return (idx(i,j));
}

} // -- namespace meshes
} // -- namespace physim
