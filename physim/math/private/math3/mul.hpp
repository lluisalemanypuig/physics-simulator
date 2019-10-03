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
#include <physim/math/private/math2/mul.hpp>

/* MULTIPLICATIONS */

// h <- f*g
#define __pm3_mul_v_v(h, f,g)				\
	__pm3_assign_c(h, (f).x*(g).x,			\
					  (f).y*(g).y,			\
					  (f).z*(g).z)
// h <- f*g*(x,y,z)
#define __pm3_mul_v_v_c(h, f,g, x,y,z)		\
	__pm3_assign_c(h, (f).x*(g).x*(x),		\
					  (f).y*(g).y*(y),		\
					  (f).z*(g).z*(z))
// h <- f*g*(s,s,s)
#define __pm3_mul_v_v_s(h, f,g, s)			\
	__pm3_assign_c(h, (f).x*(g).x*(s),		\
					  (f).y*(g).y*(s),		\
					  (f).z*(g).z*(s))
// h <- f*s
#define __pm3_mul_v_s(h, f,s)				\
	__pm3_assign_c(h, (f).x*(s),			\
					  (f).y*(s),			\
					  (f).z*(s))
// g <- g*f
#define __pm3_mul_acc_v(g, f)				\
	__pm2_mul_acc_v(g, f);					\
	(g).z *= (f).z
// g <- g*s
#define __pm3_mul_acc_s(g, s)				\
	__pm2_mul_acc_s(g, s);					\
	(g).z *= (s)
