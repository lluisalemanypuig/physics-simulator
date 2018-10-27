#pragma once

// physim includes
#include <physim/math/math_misc.hpp>

/* ADDITIONS */

// h <- f - g
#define __pm_sub_v_v(h, f,g)						\
	__pm_assign_c(h,(f).x - (g).x,					\
					(f).y - (g).y,					\
					(f).z - (g).z)
// h <- f - g - (x,y,z)
#define __pm_sub_v_v_c(h, f,g, x,y,z)				\
	__pm_assign_c(h,(f).x - (g).x - (x),			\
					(f).y - (g).y - (y),			\
					(f).z - (g).z - (z))
// h <- f - g - (x,y,z) - (s,s,s)
#define __pm_sub_v_v_c_s(h, f,g, x,y,z, s)			\
	__pm_assign_c(h,(f).x - (g).x - (x) - (s),		\
					(f).y - (g).y - (y) - (s),		\
					(f).z - (g).z - (z) - (s))
// h <- f - g - (s,s,s)
#define __pm_sub_v_v_s(h, f,g,s)					\
	__pm_assign_c(h,(f).x - (g).x - (s),			\
					(f).y - (g).y - (s),			\
					(f).z - (g).z - (s))
// h <- f - (x,y,z)
#define __pm_sub_v_c(h, f, x,y,z)					\
	__pm_assign_c(h,(f).x - (x),					\
					(f).y - (y),					\
					(f).z - (z))
// h <- f - (x,y,z) - (s,s,s)
#define __pm_sub_v_c_s(h, f, x,y,z, s)				\
	__pm_assign_c(h,(f).x - (x) - (s),				\
					(f).y - (y) - (s),				\
					(f).z - (z) - (s))
// g <- f - (s,s,s)
#define __pm_sub_v_s(g, f,s)						\
	__pm_assign_c(g,(f).x - (s),					\
					(f).y - (s),					\
					(f).z - (s))
// h <- (fx,fy,fz) - (gx,gy,gz)
#define __pm_sub_c_c(h, fx,fy,fz, gx,gy,gz)			\
	__pm_assign_c(h,(fx) - (gx),					\
					(fy) - (gy),					\
					(fz) - (gz))
// h <- (fx,fy,fz) - (gx,gy,gz) - (s,s,s)
#define __pm_sub_c_c_s(h, fx,fy,fz, gx,gy,gz, s)	\
	__pm_assign_c(h,(fx) - (gx) - (s),				\
					(fy) - (gy) - (s),				\
					(fz) - (gz) - (s))
// h <- f - g*(x,y,z)
#define __pm_sub_v_vc(h, f, g,x,y,z)				\
	__pm_assign_c(h,(f).x - (g).x*(x),				\
					(f).y - (g).y*(y),				\
					(f).z - (g).z*(z))
// h <- f - g*s
#define __pm_sub_v_vs(h, f, g,s)					\
	__pm_assign_c(h,(f).x - (g).x*(s),				\
					(f).y - (g).y*(s),				\
					(f).z - (g).z*(s))
// i <- f - g*s - h
#define __pm_sub_v_vs_v(i, f, g,s, h)				\
	__pm_assign_c(i,(f).x - (g).x*(s) - (h).x,		\
					(f).y - (g).y*(s) - (h).y,		\
					(f).z - (g).z*(s) - (h).z)
// h <- f - g*s - (x,y,z)
#define __pm_sub_v_vs_c(h, f, g,s, x,y,z)			\
	__pm_assign_c(h,(f).x - (g).x*(s) - x,			\
					(f).y - (g).y*(s) - y,			\
					(f).z - (g).z*(s) - z)
// h <- f - g*s1 - (s2,s2,s2)
#define __pm_sub_v_vs_s(h, f, g,s1, s2)				\
	__pm_assign_c(h,(f).x - (g).x*(s1) - (s2),		\
					(f).y - (g).y*(s1) - (s2),		\
					(f).z - (g).z*(s1) - (s2))
// h <- f*s1 - g*s2
#define __pm_sub_vs_vs(h, f,s1, g,s2)				\
	__pm_assign_c(h,(f).x*(s1) - (g).x*(s2),		\
					(f).y*(s1) - (g).y*(s2),		\
					(f).z*(s1) - (g).z*(s2))
// i <- f*s1 - g*s2 - h
#define __pm_sub_vs_vs_v(i, f,s1, g,s2, h)			\
	__pm_assign_c(i,(f).x*(s1) - (g).x*(s2) - (h).x,\
					(f).y*(s1) - (g).y*(s2) - (h).y,\
					(f).z*(s1) - (g).z*(s2) - (h).z)
// h <- f*s1 - g*s2 - (x,y,z)
#define __pm_sub_vs_vs_c(h, f,s1, g,s2, x,y,z)		\
	__pm_assign_c(h,(f).x*(s1) - (g).x*(s2) - (x),	\
					(f).y*(s1) - (g).y*(s2) - (y),	\
					(f).z*(s1) - (g).z*(s2) - (z))
// h <- f*s1 - g*s2 - (s3,s3,s3)
#define __pm_sub_vs_vs_s(h, f,s1, g,s2, s3)			\
	__pm_assign_c(h,(f).x*(s1) - (g).x*(s2) - (s3),	\
					(f).y*(s1) - (g).y*(s2) - (s3),	\
					(f).z*(s1) - (g).z*(s2) - (s3))
// f <- f - g
#define __pm_sub_acc_v(f,g)							\
	(f).x -= (g).x;									\
	(f).y -= (g).y;									\
	(f).z -= (g).z
// f <- f - g - (x,y,z)
#define __pm_sub_acc_v_c(f,g, x,y,z)				\
	(f).x -= ((g).x + (x));							\
	(f).y -= ((g).y + (y));							\
	(f).z -= ((g).z + (z))
// f <- f + g + (x,y,z) + (s,s,s)
#define __pm_sub_acc_v_c_s(f,g, x,y,z, s)			\
	(f).x -= ((g).x + (x) + (s));					\
	(f).y -= ((g).y + (y) + (s));					\
	(f).z -= ((g).z + (z) + (s))
// f <- f - g - (s,s,s)
#define __pm_sub_acc_v_s(f,g,s)						\
	(f).x -= ((g).x + (s));							\
	(f).y -= ((g).y + (s));							\
	(f).z -= ((g).z + (s))
// f <- f + (x,y,z)
#define __pm_sub_acc_c(f, x,y,z)					\
	(f).x += (x);									\
	(f).y += (y);									\
	(f).z += (z)
// f <- f - (x,y,z) - (s,s,s)
#define __pm_sub_acc_c_s(f, x,y,z, s)				\
	(f).x -= ((x) + (s));							\
	(f).y -= ((y) + (s));							\
	(f).z -= ((z) + (s))
// f <- f - (s,s,s)
#define __pm_sub_acc_s(f,s)							\
	(f).x -= (s);									\
	(f).y -= (s);									\
	(f).z -= (s)
// f <- f - g*(x,y,z)
#define __pm_sub_acc_vc(f, g,x,y,z)					\
	(f).x -= (g).x*(x);								\
	(f).y -= (g).y*(y);								\
	(f).z -= (g).z*(z);
// f <- f - g*s
#define __pm_sub_acc_vs(f, g,s)						\
	(f).x -= (g).x*(s);								\
	(f).y -= (g).y*(s);								\
	(f).z -= (g).z*(s);
// f <- f - g*s - h
#define __pm_sub_acc_vs_v(f, g,s, h)				\
	(f).x -= ((g).x*(s) + (h).x);					\
	(f).y -= ((g).y*(s) + (h).y);					\
	(f).z -= ((g).z*(s) + (h).z);
// f <- f - g*s - (x,y,z)
#define __pm_sub_acc_vs_c(f, g,s, x,y,z)			\
	(f).x -= ((g).x*(s) + (x));						\
	(f).y -= ((g).y*(s) + (y));						\
	(f).z -= ((g).z*(s) + (z));
// f <- f - g*s1 - (s2,s2,s2)
#define __pm_sub_acc_vs_s(f, g,s1, s2)				\
	(f).x -= ((g).x*(s1) + (s2));					\
	(f).y -= ((g).y*(s1) + (s2));					\
	(f).z -= ((g).z*(s1) + (s2))
// h <- h - f*s1 - g*s2
#define __pm_sub_acc_vs_vs(h, f,s1, g,s2)			\
	(h).x -= ((f).x*(s1) + (g).x*(s2));				\
	(h).y -= ((f).y*(s1) + (g).y*(s2));				\
	(h).z -= ((f).z*(s1) + (g).z*(s2))
// i <- i - f*s1 - g*s2 - h
#define __pm_sub_acc_vs_vs_v(i, f,s1, g,s2, h)		\
	(i).x -= ((f).x*(s1) + (g).x*(s2) + (h).x);		\
	(i).y -= ((f).y*(s1) + (g).y*(s2) + (h).y);		\
	(i).z -= ((f).z*(s1) + (g).z*(s2) + (h).z)
// h <- h - f*s1 - g*s2 - (x,y,z)
#define __pm_sub_acc_vs_vs_c(h, f,s1, g,s2, x,y,z)	\
	(h).x -= ((f).x*(s1) + (g).x*(s2) + (x));		\
	(h).y -= ((f).y*(s1) + (g).y*(s2) + (y));		\
	(h).z -= ((f).z*(s1) + (g).z*(s2) + (z))
// h <- h - f*s1 - g*s2 - (s3,s3,s3)
#define __pm_sub_acc__vs_vs_s(h, f,s1, g,s2, s3)	\
	(h).x -= ((f).x*(s1) + (g).x*(s2) + (s3));		\
	(h).y -= ((f).y*(s1) + (g).y*(s2) + (s3));		\
	(h).z -= ((f).z*(s1) + (g).z*(s2) + (s3))

