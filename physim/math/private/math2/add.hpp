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
#include <physim/math/private/math2/base.hpp>

/* ADDITIONS */

// h <- f + g
#define __pm2_add_v_v(h, f,g)							\
	__pm2_assign_c(h,(f).x + (g).x,						\
					 (f).y + (g).y)
// h <- f + g + (x,y,z)
#define __pm2_add_v_v_c(h, f,g, x,y)					\
	__pm2_assign_c(h,(f).x + (g).x + (x),				\
					 (f).y + (g).y + (y))
// h <- f + g + (x,y,z) + (s,s,s)
#define __pm2_add_v_v_c_s(h, f,g, x,y, s)				\
	__pm2_assign_c(h,(f).x + (g).x + (x) + (s),			\
					 (f).y + (g).y + (y) + (s))
// h <- f + g + (s,s,s)
#define __pm2_add_v_v_s(h, f,g,s)						\
	__pm2_assign_c(h,(f).x + (g).x + (s),				\
					 (f).y + (g).y + (s))
// h <- f + (x,y,z)
#define __pm2_add_v_c(h, f, x,y)						\
	__pm2_assign_c(h,(f).x + (x),						\
					 (f).y + (y))
// h <- f + (x,y,z) + (s,s,s)
#define __pm2_add_v_c_s(h, f, x,y, s)					\
	__pm2_assign_c(h,(f).x + (x) + (s),					\
					 (f).y + (y) + (s))
// g <- f + (s,s,s)
#define __pm2_add_v_s(g, f,s)							\
	__pm2_assign_c(g,(f).x + (s),						\
					 (f).y + (s))
// h <- (fx,fy,fz) + (gx,gy,gz)
#define __pm2_add_c_c(h, fx,fy, gx,gy)					\
	__pm2_assign_c(h,(fx) + (gx),						\
					 (fy) + (gy))
// h <- (fx,fy,fz) + (gx,gy,gz) + (s,s,s)
#define __pm2_add_c_c_s(h, fx,fy, gx,gy, s)				\
	__pm2_assign_c(h,(fx) + (gx) + (s),					\
					 (fy) + (gy) + (s))
// h <- f + g*(x,y,z)
#define __pm2_add_v_vc(h, f, g,x,y,z)					\
	__pm2_assign_c(h,(f).x + (g).x*(x),					\
					 (f).y + (g).y*(y))
// h <- f + g*s
#define __pm2_add_v_vs(h, f, g,s)						\
	__pm2_assign_c(h,(f).x + (g).x*(s),					\
					 (f).y + (g).y*(s))
// i <- f + g*s + h
#define __pm2_add_v_vs_v(i, f, g,s, h)					\
	__pm2_assign_c(i,(f).x + (g).x*(s) + (h).x,			\
					 (f).y + (g).y*(s) + (h).y)
// h <- f + g*s + (x,y,z)
#define __pm2_add_v_vs_c(h, f, g,s, x,y)				\
	__pm2_assign_c(h,(f).x + (g).x*(s) + x,				\
					 (f).y + (g).y*(s) + y)
// h <- f + g*s1 + (s2,s2,s2)
#define __pm2_add_v_vs_s(h, f, g,s1, s2)				\
	__pm2_assign_c(h,(f).x + (g).x*(s1) + (s2),			\
					 (f).y + (g).y*(s1) + (s2))
// h <- f*s1 + g*s2
#define __pm2_add_vs_vs(h, f,s1, g,s2)					\
	__pm2_assign_c(h,(f).x*(s1) + (g).x*(s2),			\
					 (f).y*(s1) + (g).y*(s2))
// i <- f*s1 + g*s2 + h
#define __pm2_add_vs_vs_v(i, f,s1, g,s2, h)				\
	__pm2_assign_c(i,(f).x*(s1) + (g).x*(s2) + (h).x,	\
					(f).y*(s1) + (g).y*(s2) + (h).y)
// h <- f*s1 + g*s2 + (x,y,z)
#define __pm2_add_vs_vs_c(h, f,s1, g,s2, x,y)			\
	__pm2_assign_c(h,(f).x*(s1) + (g).x*(s2) + (x),		\
					 (f).y*(s1) + (g).y*(s2) + (y))
// h <- f*s1 + g*s2 + (s3,s3,s3)
#define __pm2_add_vs_vs_s(h, f,s1, g,s2, s3)			\
	__pm2_assign_c(h,(f).x*(s1) + (g).x*(s2) + (s3),	\
					(f).y*(s1) + (g).y*(s2) + (s3))


// f <- f + g
#define __pm2_add_acc_v(f,g)						\
	(f).x += (g).x;									\
	(f).y += (g).y
// f <- f + g + h
#define __pm2_add_acc_v_v(f, g,h)					\
	(f).x += ((g).x + (h).x);						\
	(f).y += ((g).y + (h).y)
// f <- f + g + (x,y,z)
#define __pm2_add_acc_v_c(f,g, x,y)					\
	(f).x += ((g).x + (x));							\
	(f).y += ((g).y + (y))
// f <- f + g + (x,y,z) + (s,s,s)
#define __pm2_add_acc_v_c_s(f,g, x,y, s)			\
	(f).x += ((g).x + (x) + (s));					\
	(f).y += ((g).y + (y) + (s))
// f <- f + g + (s,s,s)
#define __pm2_add_acc_v_s(f,g,s)					\
	(f).x += ((g).x + (s));							\
	(f).y += ((g).y + (s))
// f <- f + (x,y,z)
#define __pm2_add_acc_c(f, x,y)						\
	(f).x += (x);									\
	(f).y += (y)
// f <- f + (x,y,z) + (s,s,s)
#define __pm2_add_acc_c_s(f, x,y, s)				\
	(f).x += ((x) + (s));							\
	(f).y += ((y) + (s))
// f <- f + (s,s,s)
#define __pm2_add_acc_s(f,s)						\
	(f).x += (s);									\
	(f).y += (s)
// f <- f + g*(x,y,z)
#define __pm2_add_acc_vc(f, g,x,y)					\
	(f).x += (g).x*(x);								\
	(f).y += (g).y*(y)
// f <- f + g*s
#define __pm2_add_acc_vs(f, g,s)					\
	(f).x += (g).x*(s);								\
	(f).y += (g).y*(s)
// f <- f + g*s + h
#define __pm2_add_acc_vs_v(f, g,s, h)				\
	(f).x += ((g).x*(s) + (h).x);					\
	(f).y += ((g).y*(s) + (h).y)
// f <- f + g*s + (x,y,z)
#define __pm2_add_acc_vs_c(f, g,s, x,y)				\
	(f).x += ((g).x*(s) + (x));						\
	(f).y += ((g).y*(s) + (y))
// f <- f + g*s1 + (s2,s2,s2)
#define __pm2_add_acc_vs_s(f, g,s1, s2)				\
	(f).x += ((g).x*(s1) + (s2));					\
	(f).y += ((g).y*(s1) + (s2))
// h <- h + f*s1 + g*s2
#define __pm2_add_acc_vs_vs(h, f,s1, g,s2)			\
	(h).x += ((f).x*(s1) + (g).x*(s2));				\
	(h).y += ((f).y*(s1) + (g).y*(s2))
// i <- i + f*s1 + g*s2 + h
#define __pm2_add_acc_vs_vs_v(i, f,s1, g,s2, h)		\
	(i).x += ((f).x*(s1) + (g).x*(s2) + (h).x);		\
	(i).y += ((f).y*(s1) + (g).y*(s2) + (h).y)
// h <- h + f*s1 + g*s2 + (x,y,z)
#define __pm2_add_acc_vs_vs_c(h, f,s1, g,s2, x,y)	\
	(h).x += ((f).x*(s1) + (g).x*(s2) + (x));		\
	(h).y += ((f).y*(s1) + (g).y*(s2) + (y))
// h <- h + f*s1 + g*s2 + (s3,s3,s3)
#define __pm2_add_acc_vs_vs_s(h, f,s1, g,s2, s3)	\
	(h).x += ((f).x*(s1) + (g).x*(s2) + (s3));		\
	(h).y += ((f).y*(s1) + (g).y*(s2) + (s3))
