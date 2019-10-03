/*********************************************************************
 * Real-time physics simulation project
 * Copyright (C) 2018-2019 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

#pragma once

// physim includes
#include <physim/math/private/math3/base.hpp>
#include <physim/math/private/math2/sub.hpp>

/* ADDITIONS */

// h <- f - g
#define __pm3_sub_v_v(h, f,g)							\
	__pm3_assign_c(h, (f).x - (g).x,					\
					  (f).y - (g).y,					\
					  (f).z - (g).z)
// h <- f - g - (x,y,z)
#define __pm3_sub_v_v_c(h, f,g, x,y,z)					\
	__pm3_assign_c(h, (f).x - (g).x - (x),				\
					  (f).y - (g).y - (y),				\
					  (f).z - (g).z - (z))
// h <- f - g - (x,y,z) - (s,s,s)
#define __pm3_sub_v_v_c_s(h, f,g, x,y,z, s)				\
	__pm3_assign_c(h, (f).x - (g).x - (x) - (s),		\
					  (f).y - (g).y - (y) - (s),		\
					  (f).z - (g).z - (z) - (s))
// h <- f - g - (s,s,s)
#define __pm3_sub_v_v_s(h, f,g,s)						\
	__pm3_assign_c(h, (f).x - (g).x - (s),				\
					  (f).y - (g).y - (s),				\
					  (f).z - (g).z - (s))
// h <- f - (x,y,z)
#define __pm3_sub_v_c(h, f, x,y,z)						\
	__pm3_assign_c(h, (f).x - (x),						\
					  (f).y - (y),						\
					  (f).z - (z))
// h <- f - (x,y,z) - (s,s,s)
#define __pm3_sub_v_c_s(h, f, x,y,z, s)					\
	__pm3_assign_c(h, (f).x - (x) - (s),				\
					  (f).y - (y) - (s),				\
					  (f).z - (z) - (s))
// g <- f - (s,s,s)
#define __pm3_sub_v_s(g, f,s)							\
	__pm3_assign_c(g, (f).x - (s),						\
					  (f).y - (s),						\
					  (f).z - (s))
// h <- (fx,fy,fz) - (gx,gy,gz)
#define __pm3_sub_c_c(h, fx,fy,fz, gx,gy,gz)			\
	__pm3_assign_c(h, (fx) - (gx),						\
					  (fy) - (gy),						\
					  (fz) - (gz))
// h <- (fx,fy,fz) - (gx,gy,gz) - (s,s,s)
#define __pm3_sub_c_c_s(h, fx,fy,fz, gx,gy,gz, s)		\
	__pm3_assign_c(h, (fx) - (gx) - (s),				\
					  (fy) - (gy) - (s),				\
					  (fz) - (gz) - (s))
// h <- f - g*(x,y,z)
#define __pm3_sub_v_vc(h, f, g,x,y,z)					\
	__pm3_assign_c(h, (f).x - (g).x*(x),				\
					  (f).y - (g).y*(y),				\
					  (f).z - (g).z*(z))
// h <- f - g*s
#define __pm3_sub_v_vs(h, f, g,s)						\
	__pm3_assign_c(h, (f).x - (g).x*(s),				\
					  (f).y - (g).y*(s),				\
					  (f).z - (g).z*(s))
// i <- f - g*s - h
#define __pm3_sub_v_vs_v(i, f, g,s, h)					\
	__pm3_assign_c(i, (f).x - (g).x*(s) - (h).x,		\
					  (f).y - (g).y*(s) - (h).y,		\
					  (f).z - (g).z*(s) - (h).z)
// h <- f - g*s - (x,y,z)
#define __pm3_sub_v_vs_c(h, f, g,s, x,y,z)				\
	__pm3_assign_c(h, (f).x - (g).x*(s) - x,			\
					  (f).y - (g).y*(s) - y,			\
					  (f).z - (g).z*(s) - z)
// h <- f - g*s1 - (s2,s2,s2)
#define __pm3_sub_v_vs_s(h, f, g,s1, s2)				\
	__pm3_assign_c(h, (f).x - (g).x*(s1) - (s2),		\
					  (f).y - (g).y*(s1) - (s2),		\
					  (f).z - (g).z*(s1) - (s2))
// h <- f*s1 - g*s2
#define __pm3_sub_vs_vs(h, f,s1, g,s2)					\
	__pm3_assign_c(h, (f).x*(s1) - (g).x*(s2),			\
					  (f).y*(s1) - (g).y*(s2),			\
					  (f).z*(s1) - (g).z*(s2))
// i <- f*s1 - g*s2 - h
#define __pm3_sub_vs_vs_v(i, f,s1, g,s2, h)				\
	__pm3_assign_c(i, (f).x*(s1) - (g).x*(s2) - (h).x,	\
					  (f).y*(s1) - (g).y*(s2) - (h).y,	\
					  (f).z*(s1) - (g).z*(s2) - (h).z)
// h <- f*s1 - g*s2 - (x,y,z)
#define __pm3_sub_vs_vs_c(h, f,s1, g,s2, x,y,z)			\
	__pm3_assign_c(h, (f).x*(s1) - (g).x*(s2) - (x),	\
					  (f).y*(s1) - (g).y*(s2) - (y),	\
					  (f).z*(s1) - (g).z*(s2) - (z))
// h <- f*s1 - g*s2 - (s3,s3,s3)
#define __pm3_sub_vs_vs_s(h, f,s1, g,s2, s3)			\
	__pm3_assign_c(h, (f).x*(s1) - (g).x*(s2) - (s3),	\
					  (f).y*(s1) - (g).y*(s2) - (s3),	\
					  (f).z*(s1) - (g).z*(s2) - (s3))
// f <- f - g
#define __pm3_sub_acc_v(f,g)						\
	__pm2_sub_acc_v(f, g);							\
	(f).z -= (g).z
// f <- f - g - (x,y,z)
#define __pm3_sub_acc_v_c(f,g, x,y,z)				\
	__pm2_sub_acc_v_c(f,g, x,y);					\
	(f).z -= ((g).z + (z))
// f <- f + g + (x,y,z) + (s,s,s)
#define __pm3_sub_acc_v_c_s(f,g, x,y,z, s)			\
	__pm2_sub_acc_v_c_s(f,g, x,y, s);				\
	(f).z -= ((g).z + (z) + (s))
// f <- f - g - (s,s,s)
#define __pm3_sub_acc_v_s(f,g,s)					\
	__pm2_sub_acc_v_s(f,g,s);						\
	(f).z -= ((g).z + (s))
// f <- f + (x,y,z)
#define __pm3_sub_acc_c(f, x,y,z)					\
	__pm2_sub_acc_c(f, x,y);						\
	(f).z += (z)
// f <- f - (x,y,z) - (s,s,s)
#define __pm3_sub_acc_c_s(f, x,y,z, s)				\
	__pm2_sub_acc_c_s(f, x,y, s);					\
	(f).z -= ((z) + (s))
// f <- f - (s,s,s)
#define __pm3_sub_acc_s(f,s)						\
	__pm2_sub_acc_s(f, s);							\
	(f).z -= (s)
// f <- f - g*(x,y,z)
#define __pm3_sub_acc_vc(f, g,x,y,z)				\
	__pm2_sub_acc_vc(f, g,x,y);						\
	(f).z -= (g).z*(z)
// f <- f - g*s
#define __pm3_sub_acc_vs(f, g,s)					\
	__pm2_sub_acc_vs(f, g,s);						\
	(f).z -= (g).z*(s)
// f <- f - g*s - h
#define __pm3_sub_acc_vs_v(f, g,s, h)				\
	__pm2_sub_acc_vs_v(f, g,s, h);					\
	(f).z -= ((g).z*(s) + (h).z)
// f <- f - g*s - (x,y,z)
#define __pm3_sub_acc_vs_c(f, g,s, x,y,z)			\
	__pm2_sub_acc_vs_c(f, g,s, x,y);				\
	(f).z -= ((g).z*(s) + (z))
// f <- f - g*s1 - (s2,s2,s2)
#define __pm3_sub_acc_vs_s(f, g,s1, s2)				\
	__pm2_sub_acc_vs_s(f, g,s1, s2);				\
	(f).z -= ((g).z*(s1) + (s2))
// h <- h - f*s1 - g*s2
#define __pm3_sub_acc_vs_vs(h, f,s1, g,s2)			\
	__pm2_sub_acc_vs_vs(h, f,s1, g,s2);				\
	(h).z -= ((f).z*(s1) + (g).z*(s2))
// i <- i - f*s1 - g*s2 - h
#define __pm3_sub_acc_vs_vs_v(i, f,s1, g,s2, h)		\
	__pm2_sub_acc_vs_vs_v(i, f,s1, g,s2, h);		\
	(i).z -= ((f).z*(s1) + (g).z*(s2) + (h).z)
// h <- h - f*s1 - g*s2 - (x,y,z)
#define __pm3_sub_acc_vs_vs_c(h, f,s1, g,s2, x,y,z)	\
	__pm2_sub_acc_vs_vs_c(h, f,s1, g,s2, x,y);		\
	(h).z -= ((f).z*(s1) + (g).z*(s2) + (z))
// h <- h - f*s1 - g*s2 - (s3,s3,s3)
#define __pm3_sub_acc__vs_vs_s(h, f,s1, g,s2, s3)	\
	__pm2_sub_acc__vs_vs_s(h, f,s1, g,s2, s3);		\
	(h).z -= ((f).z*(s1) + (g).z*(s2) + (s3))

