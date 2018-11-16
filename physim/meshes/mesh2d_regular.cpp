#include <physim/meshes/mesh2d_regular.hpp>

// C includes
#include <stdlib.h>
#include <assert.h>

// physim includes
#include <physim/math/private/math3.hpp>

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
	__pm3_sub_v_v(dir, ps[i + 1]->cur_pos, ps[i]->cur_pos);
	dist = __pm3_norm(dir);
	__pm3_normalise(dir, dir);
	// difference of velocities
	__pm3_sub_v_v(dvel, ps[i + 1]->cur_vel, ps[i]->cur_vel);
	// compute terms
	elastic_term = Ke*(dist - ds[i]);
	damping_term = Kd*__pm3_dot(dvel, dir);
	// compute forces
	__pm3_assign_vs(F1_m1, dir, elastic_term + damping_term);
	__pm3_add_acc_v(ps[i]->force, F1_m1);
	__pm3_invert(F1_m1, F1_m1);
	__pm3_add_acc_v(ps[i + 1]->force, F1_m1);
*/
// d: original distance between particles i and j
#define compute_forces(i,j, d)											\
	__pm3_sub_v_v(dir, ps[j]->cur_pos, ps[i]->cur_pos);					\
	dist = __pm3_norm(dir);												\
	__pm3_normalise(dir, dir);											\
	__pm3_sub_v_v(dvel, ps[j]->cur_vel, ps[i]->cur_vel);					\
	__pm3_assign_vs(F1_m1, dir, Ke*(dist - d) + Kd*__pm3_dot(dvel, dir));	\
	__pm3_add_acc_v(ps[i]->force, F1_m1);								\
	__pm3_invert(F1_m1, F1_m1);											\
	__pm3_add_acc_v(ps[j]->force, F1_m1)

// PUBLIC

mesh2d_regular::mesh2d_regular() : mesh() {
	mt = mesh_type::d2_regular;
	sb_ds = nullptr;
	R = 0;
	C = 0;
	stretch = false;
	shear = false;
	bend = false;
}

mesh2d_regular::mesh2d_regular(float ke, float kd) : mesh(ke,kd) {
	mt = mesh_type::d2_regular;
	sb_ds = nullptr;
	R = 0;
	C = 0;
	stretch = false;
	shear = false;
	bend = false;
}

mesh2d_regular::~mesh2d_regular() {
	clear();
}

// MODIFIERS

void mesh2d_regular::make_initial_state() {
	if (sb_ds != nullptr) {
		free(sb_ds);
	}
	sb_ds = (math::vec6 *)malloc(R*C*sizeof(math::vec6));

	/*
	 * Since this is done only once, 'if' statements
	 * can be tolerated here (I'm talking about the 'if (i < R - 2 ...)').
	 * In the force update, the 'if' statements can't really be avoided
	 * without doing the same loops more than twice.
	 */

	for (size_t i = 0; i < R; ++i) {
		for (size_t j = 0; j < C; ++j) {

			// stretch forces
			if (j + 1 < C) {
				sb_ds[idx(i,j)].x = __pm3_dist(ps[idx(i,j)]->cur_pos, ps[idx(i,j+1)]->cur_pos);
			}
			if (i + 1 < R) {
				sb_ds[idx(i,j)].y = __pm3_dist(ps[idx(i,j)]->cur_pos, ps[idx(i+1,j)]->cur_pos);
			}

			// bend forces
			if (j + 2 < C) {
				sb_ds[idx(i,j)].z = __pm3_dist(ps[idx(i,j)]->cur_pos, ps[idx(i,j+2)]->cur_pos);
			}
			if (i + 2 < R) {
				sb_ds[idx(i,j)].u = __pm3_dist(ps[idx(i,j)]->cur_pos, ps[idx(i+2,j)]->cur_pos);
			}

			// shear forces
			if (j + 1 < C) {
				if (i > 0 ) {
					sb_ds[idx(i,j)].v = __pm3_dist(ps[idx(i,j)]->cur_pos, ps[idx(i-1,j+1)]->cur_pos);
				}
				if (i + 1 < R) {
					sb_ds[idx(i,j)].w = __pm3_dist(ps[idx(i,j)]->cur_pos, ps[idx(i+1,j+1)]->cur_pos);
				}
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

	for (size_t i = 0; i < R; ++i) {
		for (size_t j = 0; j < C; ++j) {

			if (stretch) {
				if (j + 1 < C) {
					compute_forces( idx(i,j), idx(i,j + 1), sb_ds[idx(i,j)].x );
				}
				if (i + 1 < R) {
					compute_forces( idx(i,j), idx(i + 1,j), sb_ds[idx(i,j)].y );
				}
			}

			if (bend) {
				if (j + 2 < C) {
					compute_forces( idx(i,j), idx(i,j + 2), sb_ds[idx(i,j)].z );
				}
				if (i + 2 < R) {
					compute_forces( idx(i,j), idx(i + 2,j), sb_ds[idx(i,j)].u );
				}
			}

			if (shear) {
				if (j + 1 < C) {
					if (i > 0) {
						compute_forces( idx(i,j), idx(i - 1,j + 1), sb_ds[idx(i,j)].v );
					}
					if (i + 1 < R) {
						compute_forces( idx(i,j), idx(i + 1,j + 1), sb_ds[idx(i,j)].w );
					}
				}
			}
		}
	}

	/* The following code, although nearly unreadable,
	 * is slightly faster than the previous loop.
	 */

	/*
	// -------
	// -- 1 --
	// -------

	size_t i, j;

	// all stretch, bend and shear forces for [1,R-3]x[0,C-2]
	for (i = 1; i < R - 2; ++i) {
		for (j = 0; j < C - 2; ++j) {
			if (stretch) {
				compute_forces( idx(i,j), idx(i,  j+1), sb_ds[idx(i,j)].x );
				compute_forces( idx(i,j), idx(i+1,j  ), sb_ds[idx(i,j)].y );
			}
			if (bend) {
				compute_forces( idx(i,j), idx(i,  j+2), sb_ds[idx(i,j)].z );
				compute_forces( idx(i,j), idx(i+2,j  ), sb_ds[idx(i,j)].u );
			}
			if (shear) {
				compute_forces( idx(i,j), idx(i-1,j+1), sb_ds[idx(i,j)].v );
				compute_forces( idx(i,j), idx(i+1,j+1), sb_ds[idx(i,j)].w );
			}
		}
	}

	// -------
	// -- 2 --
	// -------

	// 2.1: first row: stretch, bend and shear
	i = 0;
	for (j = 0; j < C - 2; ++j) {
		if (stretch) {
			compute_forces( idx(i,j), idx(i,  j+1), sb_ds[idx(i,j)].x );
			compute_forces( idx(i,j), idx(i+1,j  ), sb_ds[idx(i,j)].y );
		}
		if (bend) {
			compute_forces( idx(i,j), idx(i,  j+2), sb_ds[idx(i,j)].z );
			compute_forces( idx(i,j), idx(i+2,j  ), sb_ds[idx(i,j)].u );
		}
		if (shear) {
			compute_forces( idx(i,j), idx(i+1,j+1), sb_ds[idx(i,j)].w );
		}
	}

	// 2.2: second-to-last row: stretch and shear
	i = R - 2;
	if (R > 2) {
		for (j = 0; j < C - 2; ++j) {
			if (stretch) {
				compute_forces( idx(i,j), idx(i,  j+1), sb_ds[idx(i,j)].x );
				compute_forces( idx(i,j), idx(i+1,j  ), sb_ds[idx(i,j)].y );
			}
			if (bend) {
				compute_forces( idx(i,j), idx(i,  j+2), sb_ds[idx(i,j)].z );
			}
			if (shear) {
				compute_forces( idx(i,j), idx(i-1,j+1), sb_ds[idx(i,j)].v );
				compute_forces( idx(i,j), idx(i+1,j+1), sb_ds[idx(i,j)].w );
			}
		}
	}

	// 2.3: last row: stretch, bend and shear
	i = R - 1;
	for (j = 0; j < C - 2; ++j) {
		if (stretch) {
			compute_forces( idx(i,j), idx(i,  j+1), sb_ds[idx(i,j)].x );
		}
		if (bend) {
			compute_forces( idx(i,j), idx(i,  j+2), sb_ds[idx(i,j)].z );
		}
		if (shear) {
			compute_forces( idx(i,j), idx(i-1,j+1), sb_ds[idx(i,j)].v );
		}
	}

	// -------
	// -- 3 --
	// -------

	// 3.1: stretch and shear
	j = C - 2;
	i = 0;
	if (stretch) {
		compute_forces( idx(i,j), idx(i,  j+1), sb_ds[idx(i,j)].x );
		compute_forces( idx(i,j), idx(i+1,j  ), sb_ds[idx(i,j)].y );
	}
	if (bend) {
		compute_forces( idx(i,j), idx(i+2,j  ), sb_ds[idx(i,j)].u );
	}
	if (shear) {
		compute_forces( idx(i,j), idx(i+1,j+1), sb_ds[idx(i,j)].w );
	}

	// 3.2: rows [1,R-3] in column C-2
	for (i = 1; i < R - 2; ++i) {
		if (stretch) {
			compute_forces( idx(i,j), idx(i,  j+1), sb_ds[idx(i,j)].x );
			compute_forces( idx(i,j), idx(i+1,j  ), sb_ds[idx(i,j)].y );
		}
		if (bend) {
			compute_forces( idx(i,j), idx(i+2,j  ), sb_ds[idx(i,j)].u );
		}
		if (shear) {
			compute_forces( idx(i,j), idx(i-1,j+1), sb_ds[idx(i,j)].v );
			compute_forces( idx(i,j), idx(i+1,j+1), sb_ds[idx(i,j)].w );
		}
	}

	// 3.3: second-to-last row in column C-2:
	i = R - 2;
	if (R > 2) {
		if (stretch) {
			compute_forces( idx(i,j), idx(i,j+1), sb_ds[idx(i,j)].x );
		}
		if (shear) {
			compute_forces( idx(i,j), idx(i-1,j+1), sb_ds[idx(i,j)].v );
		}
	}

	// 3.4: last row in column C-2:
	i = R - 1;
	if (stretch) {
		compute_forces( idx(i,j), idx(i,j+1), sb_ds[idx(i,j)].x );
	}
	if (shear) {
		compute_forces( idx(i,j), idx(i-1,j+1), sb_ds[idx(i,j)].v );
	}

	// -------
	// -- 4 --
	// -------

	// 4.1: last column for rows [0,R-3]
	j = C - 1;
	for (i = 0; i < R - 2; ++i) {
		if (stretch) {
			compute_forces( idx(i,j), idx(i+1,j), sb_ds[idx(i,j)].y );
		}
		if (bend) {
			compute_forces( idx(i,j), idx(i+2,j  ), sb_ds[idx(i,j)].u );
		}
	}

	// 4.2: last two rows in last column
	if (stretch) {
		i = R - 2;
		compute_forces( idx(i,j), idx(i+1,j), sb_ds[idx(i,j)].y );
	}
	*/
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

void mesh2d_regular::simulate_stretch(bool s) {
	stretch = s;
}

void mesh2d_regular::simulate_shear(bool s) {
	shear = s;
}

void mesh2d_regular::simulate_bend(bool s) {
	bend = s;
}

// GETTERS

bool mesh2d_regular::is_simulating_stretch() const {
	return stretch;
}

bool mesh2d_regular::is_simulating_shear() const {
	return shear;
}

bool mesh2d_regular::is_simulating_bend() const {
	return bend;
}

void mesh2d_regular::get_dimensions(size_t& r, size_t& c) const {
	r = R;
	c = C;
}

size_t mesh2d_regular::get_global_index(size_t i, size_t j) const {
	return (idx(i,j));
}

} // -- namespace meshes
} // -- namespace physim
