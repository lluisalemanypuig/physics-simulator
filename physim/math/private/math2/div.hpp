#pragma once

// physim includes
#include <physim/math/private/numeric.hpp>
#include <physim/math/private/math2/base.hpp>

/* DIVISIONS */

// h <- f/g
#define __pm2_div_v_v(h, f,g)				\
	__pm2_assign_c(h,(f).x*__pm_inv((g).x),	\
					 (f).y*__pm_inv((g).y))
// h <- f/s
#define __pm2_div_v_s(h, f,s)				\
	__pm2_assign_c(h,(f).x*__pm_inv(s),		\
					 (f).y*__pm_inv(s))
// g <- g/f
#define __pm2_div_acc_v(g, f)				\
	(g).x *= __pm_inv((f).x);				\
	(g).y *= __pm_inv((f).y)
// g <- g/s
#define __pm2_div_acc_s(g, s)				\
	(g).x *= __pm_inv(s);					\
	(g).y *= __pm_inv(s)
