#pragma once

// physim includes
#include <physim/math/private/numeric.hpp>
#include <physim/math/private/math3/base.hpp>
#include <physim/math/private/math2/div.hpp>

/* DIVISIONS */

// h <- f/g
#define __pm3_div_v_v(h, f,g)				\
	__pm3_assign_c(h,(f).x*__pm_inv((g).x),	\
					 (f).y*__pm_inv((g).y),	\
					 (f).z*__pm_inv((g).z))
// h <- f/s
#define __pm3_div_v_s(h, f,s)				\
	__pm3_assign_c(h,(f).x*__pm_inv(s),		\
					 (f).y*__pm_inv(s),		\
					 (f).z*__pm_inv(s))
// g <- g/f
#define __pm3_div_acc_v(g, f)				\
	__pm2_div_acc_v(g, f);					\
	(g).z *= __pm_inv((f).z)
// g <- g/s
#define __pm3_div_acc_s(g, s)				\
	__pm2_div_acc_s(g, s);					\
	(g).z *= __pm_inv(s)
