#pragma once

// C++ includes
#include <cmath>

// physim includes
#include <physim/math/math_misc.hpp>

namespace physim {
namespace math {

/* The readability of the code resulting from using these
 * macros is not too good. However, each of them has a clear
 * 'syntax', so to speak.
 *
 * First, start with '__pm', which makes reference to the
 * namespaces physim::math. Then follows the type of operation:
 * 'add', 'sub', 'mul', 'div'. That is, '__pm_add', '__pm_sub',
 * '__pm_mul', '__pm_div'. Multiplication and division between
 * two vectors are always component-wise.
 *
 * After the operation type comes the parameter specification.
 * There are two basic types of parameters, though three letters
 * are used:
 * - 'v' to indicate vector (see @ref vec3),
 * - 's' to indicate scalar, and
 * - 'c' to indicate 3 scalars.
 *
 * These letters are combined to specify the input parameters
 * and the type of operation between them. Notice that there
 * is always one parameter that is omitted: the output vector.
 * If they are separated by a '_' (e.g. 'v_v') they are related
 * with the operation specified. If they are together (e.g. 'vv')
 * they are both multiplied.
 *
 * For example, the macro:
 * * __pm_mul_v_v: indicates the component-wise multiplication
 * of two vectors.
 * * __pm_add_v_vs: indicates the component-wise addition of a
 * vector ('v') and the result of multiplying a vector by a
 * scalar ('vs').
 *
 * Some operations of the form 'u <- u _op_ v' have shortened
 * and defined in a number of macros. These were added an 'acc'
 * between the indicator of the operation and the parameters.
 * Therefore, the macros:
 * * __pm_sub_c: implements 'u <- u + (x,y,z)' which is equivalent
 * to 'u += (x,y,z)'.
 * * __pm_mul_s: implements 'u <- u*s' which is equivalent to
 * 'u *= s', for a scalar value 's'.
 *
 * Below follow the details on the defintion of each macro.
 */

/* ---------- */
/* ARITHMETIC */

/* ADDITIONS */

// h <- f + g
#define __pm_add_v_v(h, f,g)				\
	__pm_assign_c(h,(f).x + (g).x,			\
					(f).y + (g).y,			\
					(f).z + (g).z)
// h <- f + (x,y,z)
#define __pm_add_v_c(h, f, x,y,z)			\
	__pm_assign_c(h,(f).x + x,				\
					(f).y + y,				\
					(f).z + z)
// h <- (fx,fy,fz) + (gx,gy,gz)
#define __pm_add_c_c(h, fx,fy,fz, gx,gy,gz)	\
	__pm_assign_c(h,fx + gx,				\
					fy + gy,				\
					fz + gz)
// f <- f + g
#define __pm_add_acc_v(f,g)					\
	(f).x += (g).x;							\
	(f).y += (g).y;							\
	(f).z += (g).z
// f <- f + (s,s,s)
#define __pm_add_acc_s(f,s)					\
	(f).x += s;								\
	(f).y += s;								\
	(f).z += s
// f <- f + (x,y,z)
#define __pm_add_acc_c(f,x,y,z)				\
	(f).x += x;								\
	(f).y += y;								\
	(f).z += z
// g <- f + (s,s,s)
#define __pm_add_v_s(g, f,s)				\
	__pm_assign_c(g,(f).x + s,				\
					(f).y + s,				\
					(f).z + s)
// h <- f + g*s
#define __pm_add_v_vs(h, f, g,s)			\
	__pm_assign_c(h,(f).x + (g).x*s,		\
					(f).y + (g).y*s,		\
					(f).z + (g).z*s)
// h <- f*s1 + g*s2
#define __pm_add_vs_vs(h, f,s1, g,s2)		\
	__pm_assign_c(h,(f).x*s1 + (g).x*s2,	\
					(f).y*s1 + (g).y*s2,	\
					(f).z*s1 + (g).z*s2)

/* SUBSTRACTIONS */

// h <- f - g
#define __pm_sub_v_v(h, f,g)				\
	__pm_assign_c(h,(f).x - (g).x,			\
					(f).y - (g).y,			\
					(f).z - (g).z)
// h <- f - (x,y,z)
#define __pm_sub_v_c(h, f, x,y,z)			\
	__pm_assign_c(h,(f).x - x,				\
					(f).y - y,				\
					(f).z - z)
// h <- (fx,fy,fz) - (gx,gy,gz)
#define __pm_sub_c_c(h, fx,fy,fz, gx,gy,gz)	\
	__pm_assign_c(h,fx - gx,				\
					fy - gy,				\
					fz - gz)
// f <- f - g
#define __pm_sub_acc_v(f,g)					\
	(f).x -= (g).x;							\
	(f).y -= (g).y;							\
	(f).z -= (g).z
// f <- f - (s,s,s)
#define __pm_sub_acc_s(f,s)					\
	(f).x -= s;								\
	(f).y -= s;								\
	(f).z -= s
// f <- f - (x,y,z)
#define __pm_sub_acc_c(f,x,y,z)				\
	(f).x -= x;								\
	(f).y -= y;								\
	(f).z -= z
// g <- f - (s,s,s)
#define __pm_sub_v_s(g, f,s)				\
	__pm_assign_c(g,(f).x - s,				\
					(f).y - s,				\
					(f).z - s)
// h <- f - g*s
#define __pm_sub_v_vs(h, f, g,s)			\
	__pm_assign_c(h,(f).x - (g).x*s,		\
					(f).y - (g).y*s,		\
					(f).z - (g).z*s)
// h <- f*s1 - g*s2
#define __pm_sub_vs_vs(h, f,s1, g,s2)		\
	__pm_assign_c(h,(f).x*s1 - (g).x*s2,	\
					(f).y*s1 - (g).y*s2,	\
					(f).z*s1 - (g).z*s2)

// g <- -f
#define __pm_invert(g, f)					\
	__pm_assign_c(g,-(f).x,					\
					-(f).y,					\
					-(f).z)

/* MULTIPLICATIONS */

// h <- f*g
#define __pm_mul_v_v(h, f,g)				\
	__pm_assign_c(h,(f).x*(g).x,			\
					(f).y*(g).y,			\
					(f).z*(g).z)
// h <- f*s
#define __pm_mul_v_s(h, f,s)				\
	__pm_assign_c(h,(f).x*s,				\
					(f).y*s,				\
					(f).z*s)
// g <- g*f
#define __pm_mul_acc_v(g, f)				\
	(g).x *= (f).x;							\
	(g).y *= (f).y;							\
	(g).z *= (f).z
// g <- g*s
#define __pm_mul_acc_s(g, s)				\
	(g).x *= s;								\
	(g).y *= s;								\
	(g).z *= s

/* DIVISIONS */

// h <- f/g
#define __pm_div_v_v(h, f,g)				\
	__pm_assign_c(h,(f).x*(1.0f/(g).x),		\
					(f).y*(1.0f/(g).y),		\
					(f).z*(1.0f/(g).z))
// h <- f/s
#define __pm_div_v_s(h, f,s)				\
	__pm_assign_c(h,(f).x*(1.0f/s),			\
					(f).y*(1.0f/s),			\
					(f).z*(1.0f/s))
// g <- g/f
#define __pm_div_acc_v(g, f)				\
	(g).x *= (1.0f/(f).x);					\
	(g).y *= (1.0f/(f).y);					\
	(g).z *= (1.0f/(f).z)
// g <- g/s
#define __pm_div_acc_s(g, s)				\
	(g).x *= (1.0f/s);						\
	(g).y *= (1.0f/s);						\
	(g).z *= (1.0f/s)

} // -- namespace math
} // -- namespace physim
