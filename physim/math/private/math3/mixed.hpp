#pragma once

// physim includes
#include <physim/math/private/math3/base.hpp>

/* MIXED ARITHMETIC OPERATIONS */

// h <- (f + g)/(x,y,z)
#define __pm3_add_v_v_div_c(h, f,g, x,y,z)			\
	__pm3_assign_c(h, ((f).x + (g).x)*(1.0f/(x)),	\
					  ((f).y + (g).y)*(1.0f/(y)),	\
					  ((f).z + (g).z)*(1.0f/(z)))
// h <- (f + g)/s
#define __pm3_add_v_v_div_s(h, f,g, s)				\
	__pm3_assign_c(h, ((f).x + (g).x)*(1.0f/(s)),	\
					  ((f).y + (g).y)*(1.0f/(s)),	\
					  ((f).z + (g).z)*(1.0f/(s)))

// h <- (f*g)/(x,y,z)
#define __pm3_mul_v_v_div_c(h, f,g, x,y,z)			\
	__pm3_assign_c(h, ((f).x*(g).x)*(1.0f/(x)),		\
					  ((f).y*(g).y)*(1.0f/(y)),		\
					  ((f).z*(g).z)*(1.0f/(z)))
// h <- (f*g)/s
#define __pm3_mul_v_v_div_s(h, f,g, s)				\
	__pm3_assign_c(h, ((f).x*(g).x)*(1.0f/(s)),		\
					  ((f).y*(g).y)*(1.0f/(s)),		\
					  ((f).z*(g).z)*(1.0f/(s)))

// h <- (f - g)/(x,y,z)
#define __pm3_sub_v_v_div_c(h, f,g, x,y,z)			\
	__pm3_assign_c(h, ((f).x - (g).x)*(1.0f/(x)),	\
					  ((f).y - (g).y)*(1.0f/(y)),	\
					  ((f).z - (g).z)*(1.0f/(z)))
// h <- (f - g)/s
#define __pm3_sub_v_v_div_s(h, f,g, s)				\
	__pm3_assign_c(h, ((f).x - (g).x)*(1.0f/(s)),	\
					  ((f).y - (g).y)*(1.0f/(s)),	\
					  ((f).z - (g).z)*(1.0f/(s)))
