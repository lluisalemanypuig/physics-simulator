#pragma once

// physim includes
#include <physim/math/private/numeric.hpp>
#include <physim/math/private/math2/base.hpp>

/* MIXED ARITHMETIC OPERATIONS */

#define __pm2_sum(v) ((v).x + (v).y)

// h <- (f + g)/(x,y,z)
#define __pm2_add_v_v_div_c(h, f,g, x,y)		\
	__pm2_assign_c(h, ((f).x + (g).x)*inv(x),	\
					  ((f).y + (g).y)*inv(y))
// h <- (f + g)/s
#define __pm2_add_v_v_div_s(h, f,g, s)			\
	__pm2_assign_c(h, ((f).x + (g).x)*inv(s),	\
					  ((f).y + (g).y)*inv(s))

// h <- (f*g)/(x,y,z)
#define __pm2_mul_v_v_div_c(h, f,g, x,y)		\
	__pm2_assign_c(h, ((f).x*(g).x)*inv(x),		\
					  ((f).y*(g).y)*inv(y))
// h <- (f*g)/s
#define __pm2_mul_v_v_div_s(h, f,g, s)			\
	__pm2_assign_c(h, ((f).x*(g).x)*inv(s),		\
					  ((f).y*(g).y)*inv(s))

// h <- (f - g)/(x,y,z)
#define __pm2_sub_v_v_div_c(h, f,g, x,y)		\
	__pm2_assign_c(h, ((f).x - (g).x)*inv(x),	\
					  ((f).y - (g).y)*inv(y))
// h <- (f - g)/s
#define __pm2_sub_v_v_div_s(h, f,g, s)			\
	__pm2_assign_c(h, ((f).x - (g).x)*inv(s),	\
					  ((f).y - (g).y)*inv(s))
