#pragma once

// physim includes
#include <physim/math/private/math2/comparison.hpp>

// the first  bit of comp stores: u.x < v.x
// the second bit of comp stores: u.y < v.y
// the third  bit of comp stores: u.z < v.z
#define __pm3_lt(comp, u,v)					\
	__pm2_lt(comp, u,v);					\
	comp = comp | (((u).z < (v).z) << 2)

// the first  bit of comp stores: u.x <= v.x
// the second bit of comp stores: u.y <= v.y
// the third  bit of comp stores: u.z <= v.z
#define __pm3_le(comp, u,v)					\
	__pm2_le(comp, u,v);					\
	comp = comp | (((u).z <= (v).z) << 2)

// the first  bit of comp stores: u.x > v.x
// the second bit of comp stores: u.y > v.y
// the third  bit of comp stores: u.z > v.z
#define __pm3_gt(comp, u,v)					\
	__pm2_gt(comp, u,v);					\
	comp = comp | (((u).z > (v).z) << 2)

// the first  bit of comp stores: u.x >= v.x
// the second bit of comp stores: u.y >= v.y
// the third  bit of comp stores: u.z >= v.z
#define __pm3_ge(comp, u,v)					\
	__pm2_ge(comp, u,v);					\
	comp = comp | (((u).z >= (v).z) << 2)

// the first  bit of comp stores: u.x == v.x
// the second bit of comp stores: u.y == v.y
// the third  bit of comp stores: u.z == v.z
#define __pm3_eq(comp, u,v)					\
	__pm2_eq(comp, u,v);					\
	comp = comp | (((u).z == (v).z) << 2)

// equality with tolerance comparison
// the first  bit of comp stores: (u.x - v.x) <= tol
// the second bit of comp stores: (u.y - v.y) <= tol
// the third  bit of comp stores: (u.z - v.z) <= tol
#define __pm3_eq_tol(comp, u,v, tol)				\
	__pm2_eq_tol(comp, u,v, tol);					\
	comp = comp | ((((u).z - (v).z) <= tol) << 2)
