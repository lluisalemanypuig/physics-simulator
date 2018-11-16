#pragma once

// physim includes
#include <physim/math/private/math2/base.hpp>

/* DIVISIONS */

// h <- f/g
#define __pm2_div_v_v(h, f,g)				\
	__pm2_assign_c(h,(f).x*(1.0f/(g).x),	\
					 (f).y*(1.0f/(g).y))
// h <- f/s
#define __pm2_div_v_s(h, f,s)				\
	__pm2_assign_c(h,(f).x*(1.0f/(s)),		\
					 (f).y*(1.0f/(s)))
// g <- g/f
#define __pm2_div_acc_v(g, f)				\
	(g).x *= (1.0f/(f).x);					\
	(g).y *= (1.0f/(f).y)
// g <- g/s
#define __pm2_div_acc_s(g, s)				\
	(g).x *= (1.0f/(s));					\
	(g).y *= (1.0f/(s))
