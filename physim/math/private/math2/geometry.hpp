#pragma once

// C++ includes
#include <cmath>

// physim includes
#include <physim/math/private/math2/base.hpp>
#include <physim/math/private/math2/add.hpp>

/* GEOMETRICAL OPERATIONS */

// Change direction of vector to its opposite
#define __pm2_invert(g, f)					\
	__pm2_assign_c(g, -(f).x,				\
					  -(f).y)

// The expression of the dot product between two vectors.
#define __pm2_dot(f,g)				\
	((f).x*(g).x +					\
	 (f).y*(g).y)

// The expression for the square of the norm of a vector.
#define __pm2_norm2(g)	(__pm2_dot(g,g))
// The expression for the norm of a vector.
#define __pm2_norm(g)	(std::sqrt(__pm2_norm2(g)))

// The expression for the squared distance between
// two points, given their position vectors.
#define __pm2_dist2(u,v)				\
	(((u).x - (v).x)*((u).x - (v).x) +	\
	 ((u).y - (v).y)*((u).y - (v).y))
// The expression for the distance between
// two points, given their position vectors.
#define __pm2_dist(u,v) (std::sqrt(__pm2_dist2(u,v)))

// Make a perpendicular vector to 'u'.
#define __pm2_perp(v, u)		\
	__pm2_assign_c(v, (u).y,	\
					 -(u).x)

// Normalise vector 'g'.
#define __pm2_normalise(f,g)	physim::math::normalise(g,f)

// Angle between two non-unit vectors
#define __pm2_angle(u,v)			\
	std::acos(__pm2_dot(u,v)/(__pm2_norm(u)*__pm2_norm(v)))

// Angle between two unit vectors
#define __pm2_angle_unit(u,v)			\
	std::acos(__pm2_dot(u,v))

// signed area of the triangle p,q,r
#define __pm2_signed_area(p,q, r)		\
	((q.x - p.x)*(r.y - p.y) - (q.y - p.y)*(r.x - p.x))
// is the point aligned with the oriented line p,q?
#define __pm2_aligned(p,q, r)			\
	(__pm2_signed_area(p,q, r) == 0.0f)
// is the point to the left of the oriented line p,q?
#define __pm2_left(p,q, r)				\
	(__pm2_signed_area(p,q, r) > 0.0f)
// is the point aligned and/or to the left of the oriented line p,q?
#define __pm2_left_aligned(p,q, r)				\
	(__pm2_signed_area(p,q, r) >= 0.0f)
// is the point to the right of the oriented line p,q?
#define __pm2_right(p,q, r)				\
	(__pm2_signed_area(p,q, r) < 0.0f)
// is the point aligned and/or to the right of the oriented line p,q?
#define __pm2_right_aligned(p,q, r)				\
	(__pm2_signed_area(p,q, r) <= 0.0f)
