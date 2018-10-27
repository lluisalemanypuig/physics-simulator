#pragma once

// C++ includes
#include <cmath>

// physim includes
#include <physim/math/math_misc.hpp>

namespace physim {
namespace math {

/* MULTIPLICATIONS */

// h <- f*g
#define __pm_mul_v_v(h, f,g)				\
	__pm_assign_c(h,(f).x*(g).x,			\
					(f).y*(g).y,			\
					(f).z*(g).z)
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

} // -- namespace math
} // -- namespace physim
