#pragma once

namespace physim {
namespace math {

/* MISCELLANEA */

// g <- f
#define __pm_assign_v(g,f)		\
	(g).__x = (f).__x;			\
	(g).__y = (f).__y;			\
	(g).__z = (f).__z
// g <- (x,y,z)
#define __pm_assign_c(g, x,y,z)	\
	(g).__x = x;				\
	(g).__y = y;				\
	(g).__z = z
// g <- (s,s,s)
#define __pm_assign_s(g, s)		\
	(g).__x = (g).__y = (g).__z = s

// m <- min(a,b)
// (m.x,m.y,m.z) <- (min(a.x,b.x), min(a.y,b.y), min(a.z,b.z))
#define __pm_min_vec2(m, a,b)					\
	__pm_assign_c(m,std::min((a).__x,(b).__x),	\
					std::min((a).__y,(b).__y),	\
					std::min((a).__z,(b).__z))
// m <- min(a,b,c) = min(min(a,b),c)
#define __pm_min_vec3(m, a,b,c)					\
	__pm_min_vec2(m, a,b);						\
	__pm_min_vec2(m, m,c)
// m <- min(a,b,c,d) = min(min(min(a,b),c),d)
#define __pm_min_vec4(m, a,b,c,d)				\
	__pm_min_vec3(m, a,b,c);					\
	__pm_min_vec2(m, m,d)

// m <- max(a,b)
// (m.x,m.y,m.z) <- (max(a.x,b.x), max(a.y,b.y), min(a.z,b.z))
#define __pm_max_vec2(m, a,b)					\
	__pm_assign_c(m,std::max((a).__x,(b).__x),	\
					std::max((a).__y,(b).__y),	\
					std::max((a).__z,(b).__z))
// m <- max(a,b,c) = max(max(a,b),c)
#define __pm_max_vec3(m, a,b,c)					\
	__pm_max_vec2(m, a,b);						\
	__pm_max_vec2(m, m,c)
// m <- max(a,b,c,d) = max(max(max(a,b),c),d)
#define __pm_max_vec4(m, a,b,c,d)				\
	__pm_max_vec3(m, a,b,c);					\
	__pm_max_vec2(m, m,d)

// the contents of 'u' and 'v' are
// component-wise swapped
#define __pm_swap_vec(u,v)						\
	std::swap((u).__x,(v).__x);					\
	std::swap((u).__y,(v).__y);					\
	std::swap((u).__z,(v).__z)

// the expression for the equality
// of two vectors
#define __pm_vec_eq(u,v)						\
	(((u).__x == (v).__x) &&					\
	 ((u).__y == (v).__y) &&					\
	 ((u).__z == (v).__z))

} // -- namespace math
} // -- namespace physim
