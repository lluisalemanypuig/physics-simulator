#pragma once

// C++ includes
#include <cmath>

// physim includes
#include <physim/math/math_misc.hpp>

namespace physim {
namespace math {

/* ---------- */
/* ARITHMETIC */

/* ADDITIONS */

#define __pm_add_v_v(h, f,g)					\
	__pm_assign_c(h,(f).__x + (g).__x,			\
					(f).__y + (g).__y,			\
					(f).__z + (g).__z)
#define __pm_add_v_c(h, f, cx,cy,cz)			\
	__pm_assign_c(h,(f).__x + cx,				\
					(f).__y + cy,				\
					(f).__z + cz)
#define __pm_add_c_c(h, fx,fy,fz, cx,cy,cz)		\
	__pm_assign_c(h,fx + cx,					\
					fy + cy,					\
					fz + cz)
#define __pm_add_acc_v(f,g)						\
	(f).__x += (g).__x;							\
	(f).__y += (g).__y;							\
	(f).__z += (g).__z
#define __pm_add_acc_s(f,g)						\
	(f).__x += g;								\
	(f).__y += g;								\
	(f).__z += g
#define __pm_add_acc_c(f,cx,cy,cz)				\
	(f).__x += cx;								\
	(f).__y += cy;								\
	(f).__z += cz
#define __pm_add_v_s(g, f,s)					\
	__pm_assign_c(g,(f).__x + s,				\
					(f).__y + s,				\
					(f).__z + s)
#define __pm_add_v_vs(h, f, g,s)				\
	__pm_assign_c(h,(f).__x + (g).__x*s,		\
					(f).__y + (g).__y*s,		\
					(f).__z + (g).__z*s)
#define __pm_add_vs_vs(h, f,s1, g,s2)			\
	__pm_assign_c(h,(f).__x*s1 + (g).__x*s2,	\
					(f).__y*s1 + (g).__y*s2,	\
					(f).__z*s1 + (g).__z*s2)

/* SUBSTRACTIONS */

#define __pm_sub_v_v(h, f,g)					\
	__pm_assign_c(h,(f).__x - (g).__x,			\
					(f).__y - (g).__y,			\
					(f).__z - (g).__z)
#define __pm_sub_v_c(h, f, cx,cy,cz)			\
	__pm_assign_c(h,(f).__x - cx,				\
					(f).__y - cy,				\
					(f).__z - cz)
#define __pm_sub_c_c(h, fx,fy,fz, cx,cy,cz)		\
	__pm_assign_c(h,fx - cx,					\
					fy - cy,					\
					fz - cz)
#define __pm_sub_acc_v(f,g)						\
	(f).__x -= (g).__x;							\
	(f).__y -= (g).__y;							\
	(f).__z -= (g).__z
#define __pm_sub_acc_s(f,s)						\
	(f).__x -= s;								\
	(f).__y -= s;								\
	(f).__z -= s
#define __pm_sub_acc_c(f,cx,cy,cz)				\
	(f).__x -= cx;								\
	(f).__y -= cy;								\
	(f).__z -= cz
#define __pm_sub_v_s(g, f,s)					\
	__pm_assign_c(g,(f).__x - s,				\
					(f).__y - s,				\
					(f).__z - s)
#define __pm_sub_v_vs(h, f, g,s)				\
	__pm_assign_c(h,(f).__x - (g).__x*s,		\
					(f).__y - (g).__y*s,		\
					(f).__z - (g).__z*s)
#define __pm_sub_vs_vs(h, f,s1, g,s2)			\
	__pm_assign_c(h,(f).__x*s1 - (g).__x*s2,	\
					(f).__y*s1 - (g).__y*s2,	\
					(f).__z*s1 - (g).__z*s2)

#define __pm_conjugate(g, f)					\
	__pm_assign_c(g,-(f).__x,					\
					-(f).__y,					\
					-(f).__x)

/* MULTIPLICATIONS */

#define __pm_mul_v_v(h, f,g)					\
	__pm_assign_c(h,(f).__x*(g).__x,			\
					(f).__y*(g).__y,			\
					(f).__z*(g).__z)
#define __pm_mul_v_s(h, f,g)					\
	__pm_assign_c(h,(f).__x*g,					\
					(f).__y*g,					\
					(f).__z*g)
#define __pm_mul_acc_v(g, f)					\
	(g).__x *= (f).__x;							\
	(g).__y *= (f).__y;							\
	(g).__z *= (f).__z
#define __pm_mul_acc_s(g, f)					\
	(g).__x *= f;								\
	(g).__y *= f;								\
	(g).__z *= f

/* DIVISIONS */

#define __pm_div_v_v(h, f,g)					\
	__pm_assign_c(h,(f).__x*(1.0f/(g).__x),		\
					(f).__y*(1.0f/(g).__y),		\
					(f).__z*(1.0f/(g).__z))
#define __pm_div_v_s(h, f,g)					\
	__pm_assign_c(h,(f).__x*(1.0f/g),			\
					(f).__y*(1.0f/g),			\
					(f).__z*(1.0f/g))
#define __pm_div_acc_v(g, f)					\
	(g).__x *= (1.0f/(f).__x);					\
	(g).__y *= (1.0f/(f).__y);					\
	(g).__z *= (1.0f/(f).__z)
#define __pm_div_acc_s(g, f)					\
	(g).__x *= (1.0f/f);						\
	(g).__y *= (1.0f/f);						\
	(g).__z *= (1.0f/f)

} // -- namespace math
} // -- namespace physim
