#pragma once

// physim includes
#include <physim/math/private/math2/comparison.hpp>
#include <physim/math/private/math3/base.hpp>

// the first  bit of comp stores: u.x < v.x
// the second bit of comp stores: u.y < v.y
// the third  bit of comp stores: u.z < v.z
#define __pm3_lt(comp, u,v)					\
	__pm2_lt(comp, u,v);					\
	comp = comp | static_cast<decltype(comp)>((((u).z < (v).z) << 2))

// the first  bit of comp stores: u.x <= v.x
// the second bit of comp stores: u.y <= v.y
// the third  bit of comp stores: u.z <= v.z
#define __pm3_le(comp, u,v)					\
	__pm2_le(comp, u,v);					\
	comp = comp | static_cast<decltype(comp)>((((u).z <= (v).z) << 2))

// the first  bit of comp stores: u.x > v.x
// the second bit of comp stores: u.y > v.y
// the third  bit of comp stores: u.z > v.z
#define __pm3_gt(comp, u,v)					\
	__pm2_gt(comp, u,v);					\
	comp = comp | static_cast<decltype(comp)>((((u).z > (v).z) << 2))

// the first  bit of comp stores: u.x >= v.x
// the second bit of comp stores: u.y >= v.y
// the third  bit of comp stores: u.z >= v.z
#define __pm3_ge(comp, u,v)					\
	__pm2_ge(comp, u,v);					\
	comp = comp | static_cast<decltype(comp)>((((u).z >= (v).z) << 2))

// the first  bit of comp stores: u.x == v.x
// the second bit of comp stores: u.y == v.y
// the third  bit of comp stores: u.z == v.z
#define __pm3_eq(comp, u,v)					\
	__pm2_eq(comp, u,v);					\
	comp = comp | static_cast<decltype(comp)>((((u).z == (v).z) << 2))

// equality with tolerance comparison
// the first  bit of comp stores: (u.x - v.x) <= tol
// the second bit of comp stores: (u.y - v.y) <= tol
// the third  bit of comp stores: (u.z - v.z) <= tol
#define __pm3_eq_tol(comp, u,v, tol)				\
	__pm2_eq_tol(comp, u,v, tol);					\
	comp = comp | static_cast<decltype(comp)>(((((u).z - (v).z) <= tol) << 2))

// equality with tolerance comparison
#define __pm3_eq_tol_single(u,v, tol)				\
	(__pm2_eq_tol_single(u,v, tol)) and (((u).z - (v).z) <= tol)

// the expression for the equality
// of two vectors
#define __pm3_vec_eq(u,v)						\
	(__pm2_vec_eq(u,v) &&						\
	 ((u).z == (v).z))

// m <- min(a,b)
// (m.x,m.y,m.z) <- (min(a.x,b.x), min(a.y,b.y), min(a.z,b.z))
#define __pm3_min2(m, a,b)						\
	__pm3_assign_c(m, std::min((a).x,(b).x),	\
					  std::min((a).y,(b).y),	\
					  std::min((a).z,(b).z))
// m <- min(a,b,c) = min(min(a,b),c)
#define __pm3_min3(m, a,b,c)					\
	__pm3_min2(m, a,b);							\
	__pm3_min2(m, m,c)
// m <- min(a,b,c,d) = min(min(min(a,b),c),d)
#define __pm3_min4(m, a,b,c,d)					\
	__pm3_min3(m, a,b,c);						\
	__pm3_min2(m, m,d)

// m <- max(a,b)
// (m.x,m.y,m.z) <- (max(a.x,b.x), max(a.y,b.y), min(a.z,b.z))
#define __pm3_max2(m, a,b)						\
	__pm3_assign_c(m, std::max((a).x,(b).x),	\
					  std::max((a).y,(b).y),	\
					  std::max((a).z,(b).z))
// m <- max(a,b,c) = max(max(a,b),c)
#define __pm3_max3(m, a,b,c)					\
	__pm3_max2(m, a,b);							\
	__pm3_max2(m, m,c)
// m <- max(a,b,c,d) = max(max(max(a,b),c),d)
#define __pm3_max4(m, a,b,c,d)					\
	__pm3_max3(m, a,b,c);						\
	__pm3_max2(m, m,d)
