#pragma once

// physim includes
#include <physim/math/math_misc.hpp>

/* MULTIPLICATIONS */

// h <- f*g
#define __pm_mul_v_v(h, f,g)				\
	__pm_assign_c(h,(f).x*(g).x,			\
					(f).y*(g).y,			\
					(f).z*(g).z)
// h <- f*g*(x,y,z)
#define __pm_mul_v_v_c(h, f,g, x,y,z)		\
	__pm_assign_c(h,(f).x*(g).x*x,			\
					(f).y*(g).y*y,			\
					(f).z*(g).z*z)
// h <- f*g*(s,s,s)
#define __pm_mul_v_v_s(h, f,g, s)			\
	__pm_assign_c(h,(f).x*(g).x*s,			\
					(f).y*(g).y*s,			\
					(f).z*(g).z*s)
// h <- f*s
#define __pm_mul_v_s(h, f,s)				\
	__pm_assign_c(h,(f).x*(s),				\
					(f).y*(s),				\
					(f).z*(s))
// g <- g*f
#define __pm_mul_acc_v(g, f)				\
	(g).x *= (f).x;							\
	(g).y *= (f).y;							\
	(g).z *= (f).z
// g <- g*s
#define __pm_mul_acc_s(g, s)				\
	(g).x *= (s);							\
	(g).y *= (s);							\
	(g).z *= (s)
