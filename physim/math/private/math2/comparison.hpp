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

// the first  bit of comp stores: u.x < v.x
// the second bit of comp stores: u.y < v.y
#define __pm2_lt(comp, u,v)												\
	comp = comp | static_cast<decltype(comp)>(((u).x < (v).x));			\
	comp = comp | static_cast<decltype(comp)>((((u).y < (v).y) << 1))

// the first  bit of comp stores: u.x <= v.x
// the second bit of comp stores: u.y <= v.y
#define __pm2_le(comp, u,v)												\
	comp = comp | static_cast<decltype(comp)>(((u).x <= (v).x));		\
	comp = comp | static_cast<decltype(comp)>((((u).y <= (v).y) << 1))

// the first  bit of comp stores: u.x > v.x
// the second bit of comp stores: u.y > v.y
#define __pm2_gt(comp, u,v)												\
	comp = comp | static_cast<decltype(comp)>(((u).x > (v).x));			\
	comp = comp | static_cast<decltype(comp)>((((u).y > (v).y) << 1))

// the first  bit of comp stores: u.x >= v.x
// the second bit of comp stores: u.y >= v.y
#define __pm2_ge(comp, u,v)												\
	comp = comp | static_cast<decltype(comp)>(((u).x >= (v).x));		\
	comp = comp | static_cast<decltype(comp)>((((u).y >= (v).y) << 1))

// the first  bit of comp stores: u.x == v.x
// the second bit of comp stores: u.y == v.y
#define __pm2_eq(comp, u,v)												\
	comp = comp | static_cast<decltype(comp)>(((u).x == (v).x));		\
	comp = comp | static_cast<decltype(comp)>((((u).y == (v).y) << 1))

// equality with tolerance comparison
// the first  bit of comp stores: (u.x - v.x) <= tol
// the second bit of comp stores: (u.y - v.y) <= tol
#define __pm2_eq_tol(comp, u,v, tol)										\
	comp = comp | static_cast<decltype(comp)>((((u).x - (v).x) <= tol));	\
	comp = comp | static_cast<decltype(comp)>(((((u).y - (v).y) <= tol) << 1))

// equality with tolerance comparison
#define __pm2_eq_tol_single(u,v, tol)				\
	(((u).x - (v).x) <= tol) and (((u).y - (v).y) <= tol)

// the expression for the equality
// of two vectors
#define __pm2_vec_eq(u,v)						\
	(((u).x == (v).x) &&						\
	 ((u).y == (v).y))

// m <- min(a,b)
// (m.x,m.y,m.z) <- (min(a.x,b.x), min(a.y,b.y), min(a.z,b.z))
#define __pm2_min2(m, a,b)						\
	__pm2_assign_c(m, std::min((a).x,(b).x),	\
					  std::min((a).y,(b).y))
// m <- min(a,b,c) = min(min(a,b),c)
#define __pm2_min3(m, a,b,c)						\
	__pm2_assign_c(m,								\
		std::min( std::min((a).x, (b).x), (c).x ),	\
		std::min( std::min((a).y, (b).y), (c).y ))
// m <- min(a,b,c,d) = min(min(min(a,b),c),d)
#define __pm2_min4(m, a,b,c,d)											\
	__pm2_assign_c(m,													\
		std::min( std::min((a).x, (b).x), std::min((c).x, (d).x ) ),	\
		std::min( std::min((a).y, (b).y), std::min((c).y, (d).y ) ))

// m <- max(a,b)
// (m.x,m.y,m.z) <- (max(a.x,b.x), max(a.y,b.y), max(a.z,b.z))
#define __pm2_max2(m, a,b)						\
	__pm2_assign_c(m, std::max((a).x,(b).x),	\
					  std::max((a).y,(b).y))
// m <- max(a,b,c) = max(max(a,b),c)
#define __pm2_max3(m, a,b,c)						\
	__pm2_assign_c(m,								\
		std::max( std::max((a).x, (b).x), (c).x ),	\
		std::max( std::max((a).y, (b).y), (c).y ))
// m <- max(a,b,c,d) = max(max(max(a,b),c),d)
#define __pm2_max4(m, a,b,c,d)											\
	__pm2_assign_c(m,													\
		std::max( std::max((a).x, (b).x), std::max((c).x, (d).x ) ),	\
		std::max( std::max((a).y, (b).y), std::max((c).y, (d).y ) ))

// is point p inside box defined
// by minimum point 'm' and maximum point 'M'?
#define __pm2_inside_box(p, m,M)	\
	((((m).x <= (p).x) and ((p).x <= (M).x)) and (((m).y <= (p).y) and ((p).y <= (M).y)))
