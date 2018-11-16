#pragma once

// C++ includes
#include <cmath>

// physim includes
#include <physim/math/private/math3/base.hpp>
#include <physim/math/private/math3/add.hpp>
#include <physim/math/private/math2/geometry.hpp>

/* GEOMETRICAL OPERATIONS */

// Change direction of vector to its opposite
#define __pm3_invert(g, f)					\
	__pm3_assign_c(g, -(f).x,				\
					  -(f).y,				\
					  -(f).z)

// The expression of the dot product between two vectors.
#define __pm3_dot(f,g)				\
	(__pm2_dot(f,g) +				\
	 (f).z*(g).z)

// The expression for the square of the norm of a vector.
#define __pm3_norm2(g)	(__pm3_dot(g,g))
// The expression for the norm of a vector.
#define __pm3_norm(g)	(std::sqrt((__pm3_norm2(g))))

// The expression for the squared distance between
// two points, given their position vectors.
#define __pm3_dist2(u,v)					\
	(__pm2_dist2(u,v) +						\
	 ((u).z - (v).z)*((u).z - (v).z))
// The expression for the distance between
// two points, given their position vectors.
#define __pm3_dist(u,v) (std::sqrt(__pm3_dist2(u,v)))

// The computation of the cross product of vectors 'f' and 'g'.
// Result is stored in 'h'. Do not use this macro as
// '__pm3_cross(f, f,g)' since the result will not be correct.
#define __pm3_cross(h, f,g)						\
	__pm3_assign_c(h, (f).y*(g).z - (f).z*(g).y,	\
					 (f).z*(g).x - (f).x*(g).z,	\
					 (f).x*(g).y - (f).y*(g).x)

// The computation of the cross product of the difference of
// two vectors with respect respect to a first vector.
// That is, compute
//     i <- (v1 - v0)x(v2 - v0)
//          f = v1 - v0
//              (v1).x - (v0).x
//              (v1).y - (v0).y
//              (v1).z - (v0).z
//          g = v2 - v0
//              (v2).x - (v0).x
//              (v2).y - (v0).y
//              (v2).z - (v0).z
#define __pm3_cross_diff(i, v0,v1,v2)													\
	__pm3_assign_c(																		\
	i,	(((v1).y - (v0).y)*((v2).z - (v0).z)) - (((v1).z - (v0).z)*((v2).y - (v0).y)),	\
		(((v1).z - (v0).z)*((v2).x - (v0).x)) - (((v1).x - (v0).x)*((v2).z - (v0).z)),	\
		(((v1).x - (v0).x)*((v2).y - (v0).y)) - (((v1).y - (v0).y)*((v2).x - (v0).x)))

// Make a perpendicular vector to 'u'.
#define __pm3_perp(v, u)		\
	__pm3_assign_c(v, (u).y,	\
					 -(u).x,	\
					   0.0f)

// Normalise vector 'g'.
#define __pm3_normalise(f,g)	physim::math::normalise(g,f)

// Angle between two non-unit vectors
#define __pm3_angle(u,v)			\
	std::acos(__pm3_dot(u,v)/(__pm3_norm(u)*__pm3_norm(v)))

// Angle between two unit vectors
#define __pm3_angle_unit(u,v)			\
	std::acos(__pm3_dot(u,v))
