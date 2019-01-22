#pragma once

// physim includes
#include <physim/math/private/numeric.hpp>
#include <physim/math/private/math3/base.hpp>
#include <physim/math/private/math2/mixed.hpp>

/* MIXED ARITHMETIC OPERATIONS */

// v.x + v.y + v.z
#define __pm3_sum(v) (__pm2_sum(v) + (v).z)

// h <- (f + g)/(x,y,z)
#define __pm3_add_v_v_div_c(h, f,g, x,y,z)			\
	__pm3_assign_c(h, ((f).x + (g).x)*__pm_inv(x),	\
					  ((f).y + (g).y)*__pm_inv(y),	\
					  ((f).z + (g).z)*__pm_inv(z)
// h <- (f + g)/s
#define __pm3_add_v_v_div_s(h, f,g, s)				\
	__pm3_assign_c(h, ((f).x + (g).x)*__pm_inv(s),	\
					  ((f).y + (g).y)*__pm_inv(s),	\
					  ((f).z + (g).z)*__pm_inv(s))

// h <- (f*g)/(x,y,z)
#define __pm3_mul_v_v_div_c(h, f,g, x,y,z)			\
	__pm3_assign_c(h, ((f).x*(g).x)*__pm_inv(x),	\
					  ((f).y*(g).y)*__pm_inv(y),	\
					  ((f).z*(g).z)*__pm_inv(z)
// h <- (f*g)/s
#define __pm3_mul_v_v_div_s(h, f,g, s)				\
	__pm3_assign_c(h, ((f).x*(g).x)*__pm_inv(s),	\
					  ((f).y*(g).y)*__pm_inv(s),	\
					  ((f).z*(g).z)*__pm_inv(s))

// h <- (f - g)*(x,y,z)
#define __pm3_sub_v_v_mul_c(h, f,g, x,y,z)		\
	__pm3_assign_c(h, ((f).x - (g).x)*(x),		\
					  ((f).y - (g).y)*(y),		\
					  ((f).z - (g).z)*(z))
// h <- (f - g)*s
#define __pm3_sub_v_v_mul_s(h, f,g, s)			\
	__pm3_assign_c(h, ((f).x - (g).x)*(s),		\
					  ((f).y - (g).y)*(s),		\
					  ((f).z - (g).z)*(s))

// h <- (f - g)/(x,y,z)
#define __pm3_sub_v_v_div_c(h, f,g, x,y,z)			\
	__pm3_assign_c(h, ((f).x - (g).x)*__pm_inv(x),	\
					  ((f).y - (g).y)*__pm_inv(y),	\
					  ((f).z - (g).z)*__pm_inv(z)
// h <- (f - g)/s
#define __pm3_sub_v_v_div_s(h, f,g, s)				\
	__pm3_assign_c(h, ((f).x - (g).x)*__pm_inv(s),	\
					  ((f).y - (g).y)*__pm_inv(s),	\
					  ((f).z - (g).z)*__pm_inv(s))
