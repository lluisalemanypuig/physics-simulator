#pragma once

namespace physim {
namespace math {

/* MISCELLANEA */

#define __pm_assign_v(g,f)		\
	(g).__x = (f).__x;			\
	(g).__y = (f).__y;			\
	(g).__z = (f).__z
#define __pm_assign_c(g, x,y,z)	\
	(g).__x = x;				\
	(g).__y = y;				\
	(g).__z = z
#define __pm_assign_s(g, s)		\
	(g).__x = (g).__y = (g).__z = s

#define __pm_min_vec2(m, a,b)					\
	__pm_assign_c(m,std::min((a).__x,(b).__x),	\
					std::min((a).__y,(b).__y),	\
					std::min((a).__z,(b).__z))
#define __pm_min_vec3(m, a,b,c)					\
	__pm_min_vec2(m, a,b);						\
	__pm_min_vec2(m, m,c)
#define __pm_min_vec4(m, a,b,c,d)				\
	__pm_min_vec3(m, a,b,c);					\
	__pm_min_vec2(m, m,d)

#define __pm_max_vec2(m, a,b)					\
	__pm_assign_c(m,std::max((a).__x,(b).__x),	\
					std::max((a).__y,(b).__y),	\
					std::max((a).__z,(b).__z))
#define __pm_max_vec3(m, a,b,c)					\
	__pm_max_vec2(m, a,b);						\
	__pm_max_vec2(m, m,c)
#define __pm_max_vec4(m, a,b,c,d)				\
	__pm_max_vec3(m, a,b,c);					\
	__pm_max_vec2(m, m,d)

#define __pm_swap_vec(u,v)						\
	std::swap((u).__x,(v).__x);					\
	std::swap((u).__y,(v).__y);					\
	std::swap((u).__z,(v).__z)

#define __pm_vec_eq(u,v)						\
	(((u).__x == (v).__x) &&					\
	 ((u).__y == (v).__y) &&					\
	 ((u).__z == (v).__z))

} // -- namespace math
} // -- namespace physim
