#pragma once

// C++ includes
#include <cmath>

// physim includes
#include <physim/math/math_misc.hpp>

namespace physim {
namespace math {

/* DIVISIONS */

// h <- f/g
#define __pm_div_v_v(h, f,g)				\
	__pm_assign_c(h,(f).x*(1.0f/(g).x),		\
					(f).y*(1.0f/(g).y),		\
					(f).z*(1.0f/(g).z))
// h <- f/s
#define __pm_div_v_s(h, f,s)				\
	__pm_assign_c(h,(f).x*(1.0f/(s)),		\
					(f).y*(1.0f/(s)),		\
					(f).z*(1.0f/(s)))
// g <- g/f
#define __pm_div_acc_v(g, f)				\
	(g).x *= (1.0f/(f).x);					\
	(g).y *= (1.0f/(f).y);					\
	(g).z *= (1.0f/(f).z)
// g <- g/s
#define __pm_div_acc_s(g, s)				\
	(g).x *= (1.0f/(s));					\
	(g).y *= (1.0f/(s));					\
	(g).z *= (1.0f/(s))

} // -- namespace math
} // -- namespace physim
