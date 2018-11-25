#pragma once

namespace physim {
namespace math {

/* MISCELLANEA */

// output a vec3
#define __pm2_out(v) (v).x << "," << (v).y

// g <- f
#define __pm2_assign_v(g,f)			\
	(g).x = (f).x;					\
	(g).y = (f).y
// g <- (x,y,z)
#define __pm2_assign_c(g, a,b)		\
	(g).x = a;						\
	(g).y = b
// g <- (s,s,s)
#define __pm2_assign_s(g, s)		\
	(g).x = (g).y = s
// h <- f*g
#define __pm2_assign_vv(h,f,g)		\
	(g).x = (f).x*(g).x;			\
	(g).y = (f).y*(g).y
// g <- f*(x,y,z)
#define __pm2_assign_vc(g,f,x,y)	\
	(g).x = (f).x*(x);				\
	(g).y = (f).y*(y)
// g <- f*s
#define __pm2_assign_vs(g,f,s)		\
	(g).x = (f).x*(s);				\
	(g).y = (f).y*(s)

// m <- min(a,b)
// (m.x,m.y,m.z) <- (min(a.x,b.x), min(a.y,b.y), min(a.z,b.z))
#define __pm2_min_vec2(m, a,b)					\
	__pm2_assign_c(m,std::min((a).x,(b).x),		\
					 std::min((a).y,(b).y))
// m <- min(a,b,c) = min(min(a,b),c)
#define __pm2_min_vec3(m, a,b,c)				\
	__pm2_min_vec2(m, a,b);						\
	__pm2_min_vec2(m, m,c)
// m <- min(a,b,c,d) = min(min(min(a,b),c),d)
#define __pm2_min_vec4(m, a,b,c,d)				\
	__pm2_min_vec3(m, a,b,c);					\
	__pm2_min_vec2(m, m,d)

// m <- max(a,b)
// (m.x,m.y,m.z) <- (max(a.x,b.x), max(a.y,b.y), min(a.z,b.z))
#define __pm2_max_vec2(m, a,b)					\
	__pm2_assign_c(m,std::max((a).x,(b).x),		\
					 std::max((a).y,(b).y))
// m <- max(a,b,c) = max(max(a,b),c)
#define __pm2_max_vec3(m, a,b,c)				\
	__pm2_max_vec2(m, a,b);						\
	__pm2_max_vec2(m, m,c)
// m <- max(a,b,c,d) = max(max(max(a,b),c),d)
#define __pm2_max_vec4(m, a,b,c,d)				\
	__pm2_max_vec3(m, a,b,c);					\
	__pm2_max_vec2(m, m,d)

// the contents of 'u' and 'v' are
// component-wise swapped
#define __pm2_swap_vec(u,v)						\
	std::swap((u).x,(v).x);						\
	std::swap((u).y,(v).y)

// the expression for the equality
// of two vectors
#define __pm2_vec_eq(u,v)						\
	(((u).x == (v).x) &&						\
	 ((u).y == (v).y))

} // -- namespace math
} // -- namespace physim
