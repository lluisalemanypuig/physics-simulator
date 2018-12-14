#pragma once

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

// the contents of 'u' and 'v' are
// component-wise swapped
#define __pm2_swap_vec(u,v)			\
	std::swap((u).x,(v).x);			\
	std::swap((u).y,(v).y)
