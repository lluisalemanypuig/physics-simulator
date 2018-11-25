#pragma once

// physim includes
#include <physim/math/private/math2/base.hpp>

namespace physim {
namespace math {

/* MISCELLANEA */

// output a vec3
#define __pm3_out(v) __pm2_out(v) << "," << (v).z

// g <- f
#define __pm3_assign_v(g,f)			\
	__pm2_assign_v(g,f);			\
	(g).z = (f).z
// g <- (x,y,z)
#define __pm3_assign_c(g, a,b,c)	\
	__pm2_assign_c(g, a,b);			\
	(g).z = c
// g <- (s,s,s)
#define __pm3_assign_s(g, s)		\
	(g).x = (g).y = (g).z = s
// h <- f*g
#define __pm3_assign_vv(h,f,g)		\
	__pm2_assign_vv(h,f,g);			\
	(g).z = (f).z*(g).z
// g <- f*(x,y,z)
#define __pm3_assign_vc(g,f, x,y,z)	\
	__pm2_assign_vc(g,f, x,y);		\
	(g).z = (f).z*(z)
// g <- f*s
#define __pm3_assign_vs(g,f, s)		\
	__pm2_assign_vs(g,f, s);		\
	(g).z = (f).z*(s)

// m <- min(a,b)
// (m.x,m.y,m.z) <- (min(a.x,b.x), min(a.y,b.y), min(a.z,b.z))
#define __pm3_min_vec2(m, a,b)					\
	__pm3_assign_c(m, std::min((a).x,(b).x),	\
					  std::min((a).y,(b).y),	\
					  std::min((a).z,(b).z))
// m <- min(a,b,c) = min(min(a,b),c)
#define __pm3_min_vec3(m, a,b,c)				\
	__pm3_min_vec2(m, a,b);						\
	__pm3_min_vec2(m, m,c)
// m <- min(a,b,c,d) = min(min(min(a,b),c),d)
#define __pm3_min_vec4(m, a,b,c,d)				\
	__pm3_min_vec3(m, a,b,c);					\
	__pm3_min_vec2(m, m,d)

// m <- max(a,b)
// (m.x,m.y,m.z) <- (max(a.x,b.x), max(a.y,b.y), min(a.z,b.z))
#define __pm3_max_vec2(m, a,b)					\
	__pm3_assign_c(m, std::max((a).x,(b).x),	\
					  std::max((a).y,(b).y),	\
					  std::max((a).z,(b).z))
// m <- max(a,b,c) = max(max(a,b),c)
#define __pm3_max_vec3(m, a,b,c)				\
	__pm3_max_vec2(m, a,b);						\
	__pm3_max_vec2(m, m,c)
// m <- max(a,b,c,d) = max(max(max(a,b),c),d)
#define __pm3_max_vec4(m, a,b,c,d)				\
	__pm3_max_vec3(m, a,b,c);					\
	__pm3_max_vec2(m, m,d)

// the contents of 'u' and 'v' are
// component-wise swapped
#define __pm3_swap_vec(u,v)						\
	__pm2_swap_vec(u,v);						\
	std::swap((u).z,(v).z)

// the expression for the equality
// of two vectors
#define __pm3_vec_eq(u,v)						\
	(__pm2_vec_eq(u,v) &&						\
	 ((u).z == (v).z))

} // -- namespace math
} // -- namespace physim
