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
#include <physim/math/private/numeric.hpp>
#include <physim/math/private/math2/base.hpp>

/* MIXED ARITHMETIC OPERATIONS */

#define __pm2_sum(v) ((v).x + (v).y)

// h <- (f + g)/(x,y,z)
#define __pm2_add_v_v_div_c(h, f,g, x,y)			\
	__pm2_assign_c(h, ((f).x + (g).x)*__pm_inv(x),	\
					  ((f).y + (g).y)*__pm_inv(y))
// h <- (f + g)/s
#define __pm2_add_v_v_div_s(h, f,g, s)				\
	__pm2_assign_c(h, ((f).x + (g).x)*__pm_inv(s),	\
					  ((f).y + (g).y)*__pm_inv(s))

// h <- (f*g)/(x,y,z)
#define __pm2_mul_v_v_div_c(h, f,g, x,y)			\
	__pm2_assign_c(h, ((f).x*(g).x)*__pm_inv(x),	\
					  ((f).y*(g).y)*__pm_inv(y))
// h <- (f*g)/s
#define __pm2_mul_v_v_div_s(h, f,g, s)				\
	__pm2_assign_c(h, ((f).x*(g).x)*__pm_inv(s),	\
					  ((f).y*(g).y)*__pm_inv(s))

// h <- (f - g)/(x,y,z)
#define __pm2_sub_v_v_div_c(h, f,g, x,y)			\
	__pm2_assign_c(h, ((f).x - (g).x)*__pm_inv(x),	\
					  ((f).y - (g).y)*__pm_inv(y))
// h <- (f - g)/s
#define __pm2_sub_v_v_div_s(h, f,g, s)				\
	__pm2_assign_c(h, ((f).x - (g).x)*__pm_inv(s),	\
					  ((f).y - (g).y)*__pm_inv(s))
