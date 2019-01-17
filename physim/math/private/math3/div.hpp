#pragma once

// physim includes
#include <physim/math/private/numeric.hpp>
#include <physim/math/private/math3/base.hpp>
#include <physim/math/private/math2/div.hpp>

/* DIVISIONS */

// h <- f/g
#define __pm3_div_v_v(h, f,g)				\
	__pm3_assign_c(h,(f).x*inv((g).x),		\
					 (f).y*inv((g).y),		\
					 (f).z*inv((g).z))
// h <- f/s
#define __pm3_div_v_s(h, f,s)				\
	__pm3_assign_c(h,(f).x*inv(s),			\
					 (f).y*inv(s),			\
					 (f).z*inv(s))
// g <- g/f
#define __pm3_div_acc_v(g, f)				\
	__pm2_div_acc_v(g, f);					\
	(g).z *= inv((f).z)
// g <- g/s
#define __pm3_div_acc_s(g, s)				\
	__pm2_div_acc_s(g, s);					\
	(g).z *= inv(s)
