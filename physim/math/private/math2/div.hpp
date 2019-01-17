#pragma once

// physim includes
#include <physim/math/private/numeric.hpp>
#include <physim/math/private/math2/base.hpp>

/* DIVISIONS */

// h <- f/g
#define __pm2_div_v_v(h, f,g)				\
	__pm2_assign_c(h,(f).x*inv((g).x),		\
					 (f).y*inv((g).y))
// h <- f/s
#define __pm2_div_v_s(h, f,s)				\
	__pm2_assign_c(h,(f).x*inv(s),			\
					 (f).y*inv(s))
// g <- g/f
#define __pm2_div_acc_v(g, f)				\
	(g).x *= inv((f).x);					\
	(g).y *= inv((f).y)
// g <- g/s
#define __pm2_div_acc_s(g, s)				\
	(g).x *= inv(s);						\
	(g).y *= inv(s)
