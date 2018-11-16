#pragma once

// physim includes
#include <physim/math/private/math2/base.hpp>

/* MULTIPLICATIONS */

// h <- f*g
#define __pm2_mul_v_v(h, f,g)				\
	__pm2_assign_c(h, (f).x*(g).x,			\
					  (f).y*(g).y)
// h <- f*g*(x,y,z)
#define __pm2_mul_v_v_c(h, f,g, x,y)		\
	__pm2_assign_c(h, (f).x*(g).x*(x),		\
					  (f).y*(g).y*(y))
// h <- f*g*(s,s,s)
#define __pm2_mul_v_v_s(h, f,g, s)			\
	__pm2_assign_c(h, (f).x*(g).x*(s),		\
					  (f).y*(g).y*(s))
// h <- f*s
#define __pm2_mul_v_s(h, f,s)				\
	__pm2_assign_c(h, (f).x*(s),			\
					  (f).y*(s))
// g <- g*f
#define __pm2_mul_acc_v(g, f)				\
	(g).x *= (f).x;							\
	(g).y *= (f).y
// g <- g*s
#define __pm2_mul_acc_s(g, s)				\
	(g).x *= (s);							\
	(g).y *= (s)
