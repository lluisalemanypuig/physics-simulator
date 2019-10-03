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

/* DIVISIONS */

// h <- f/g
#define __pm2_div_v_v(h, f,g)				\
	__pm2_assign_c(h,(f).x*__pm_inv((g).x),	\
					 (f).y*__pm_inv((g).y))
// h <- f/s
#define __pm2_div_v_s(h, f,s)				\
	__pm2_assign_c(h,(f).x*__pm_inv(s),		\
					 (f).y*__pm_inv(s))
// g <- g/f
#define __pm2_div_acc_v(g, f)				\
	(g).x *= __pm_inv((f).x);				\
	(g).y *= __pm_inv((f).y)
// g <- g/s
#define __pm2_div_acc_s(g, s)				\
	(g).x *= __pm_inv(s);					\
	(g).y *= __pm_inv(s)
