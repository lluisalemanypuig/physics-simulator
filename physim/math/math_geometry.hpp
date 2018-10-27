#pragma once

// C++ includes
#include <cmath>

// physim includes
#include <physim/math/math_misc.hpp>
#include <physim/math/math_ops.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace math {

/* GEOMETRICAL OPERATIONS */

// The expression of the dot product between two vectors.
#define __pm_dot(f,g)				\
	((f).x*(g).x +					\
	 (f).y*(g).y +					\
	 (f).z*(g).z)

// The expression for the square of the norm of a vector.
#define __pm_norm2(g)	(__pm_dot(g,g))
// The expression for the norm of a vector.
#define __pm_norm(g)	(std::sqrt((__pm_norm2(g))))

// The expression for the squared distance between
// two points, given their position vectors.
#define __pm_dist2(u,v)					\
	(((u).x - (v).x)*((u).x - (v).x) +	\
	 ((u).y - (v).y)*((u).y - (v).y) +	\
	 ((u).z - (v).z)*((u).z - (v).z))
// The expression for the distance between
// two points, given their position vectors.
#define __pm_dist(u,v) (std::sqrt(__pm_dist2(u,v)))

// The computation of the cross product of vectors 'f' and 'g'.
// Result is stored in 'h'. Do not use this macro as
// '__pm_cross(f, f,g)' since the result will not be correct.
#define __pm_cross(h, f,g)						\
	__pm_assign_c(h, (f).y*(g).z - (f).z*(g).y,	\
					 (f).z*(g).x - (f).x*(g).z,	\
					 (f).x*(g).y - (f).y*(g).x)

// Make a perpendicular vector to 'u'.
#define __pm_perp(v, u)		\
	__pm_assign_c(v,(u).y,	\
					-(u).x,	\
					0.0f)

// Normalise vector 'g'.
#define __pm_normalise(f,g)	normalise(g,f)

} // -- namespace math
} // -- namespace physim
