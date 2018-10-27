#pragma once

// physim includes
#include <physim/math/math_misc.hpp>

namespace physim {
namespace math {

/* MIXED ARITHMETIC OPERATIONS */

// h <- (f + g)/(x,y,z)
#define __pm_add_div_v_v_c(h, f,g, x,y,z)			\
	__pm_assign_c(h, ((f).x + (g).x)*(1.0f/(x)),	\
					 ((f).y + (g).y)*(1.0f/(y)),	\
					 ((f).z + (g).z)*(1.0f/(z)))
// h <- (f + g)/s
#define __pm_add_div_v_v_s(h, f,g, s)				\
	__pm_assign_c(h, ((f).x + (g).x)*(1.0f/(s)),	\
					 ((f).y + (g).y)*(1.0f/(s)),	\
					 ((f).z + (g).z)*(1.0f/(s)))

// h <- (f - g)/(x,y,z)
#define __pm_sub_div_v_v_c(h, f,g, x,y,z)			\
	__pm_assign_c(h, ((f).x - (g).x)*(1.0f/(x)),	\
					 ((f).y - (g).y)*(1.0f/(y)),	\
					 ((f).z - (g).z)*(1.0f/(z)))
// h <- (f - g)/s
#define __pm_sub_div_v_v_s(h, f,g, s)				\
	__pm_assign_c(h, ((f).x - (g).x)*(1.0f/(s)),	\
					 ((f).y - (g).y)*(1.0f/(s)),	\
					 ((f).z - (g).z)*(1.0f/(s)))


} // -- namespace math
} // -- namespace physim
