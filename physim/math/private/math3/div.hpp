#pragma once

// physim includes
#include <physim/math/private/math3/base.hpp>
#include <physim/math/private/math2/div.hpp>

/* DIVISIONS */

// h <- f/g
#define __pm3_div_v_v(h, f,g)				\
	__pm3_assign_c(h,(f).x*(1.0f/(g).x),	\
					(f).y*(1.0f/(g).y),		\
					(f).z*(1.0f/(g).z))
// h <- f/s
#define __pm3_div_v_s(h, f,s)				\
	__pm3_assign_c(h,(f).x*(1.0f/(s)),		\
					(f).y*(1.0f/(s)),		\
					(f).z*(1.0f/(s)))
// g <- g/f
#define __pm3_div_acc_v(g, f)				\
	__pm2_div_acc_v(g, f);					\
	(g).z *= (1.0f/(f).z)
// g <- g/s
#define __pm3_div_acc_s(g, s)				\
	__pm2_div_acc_s(g, s);					\
	(g).z *= (1.0f/(s))
