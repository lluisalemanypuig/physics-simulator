#pragma once

// C++ includes
#include <fstream>
#include <cmath>

// physim includes
#include <physim/math/math_ops.hpp>
#include <physim/math/math_misc.hpp>

namespace physim {
namespace math {

/**
 * @brief 3-dimensional vector.
 *
 * Basic implementation of a three-dimensional vector as a struct
 * with only three attributes @ref vec3::__x, @ref vec3::__y and @ref vec3::__z.
 *
 * The notation is borrowed from the 'glm' library.
 */
typedef struct vec3 {
	/// First component of the vector.
	float __x;
	/// Second component of the vector.
	float __y;
	/// Third component of the vector.
	float __z;

	/// Default constructor.
	vec3() { __pm_assign_c(*this, 0.0f,0.0f,0.0f); }
	/// Constructor at point (@e p, @e p, @e p).
	vec3(float p) {
		__pm_assign_s(*this, p);
	}
	/// Construct a vector with coordinates (@e x, @e y, @e z).
	vec3(float x,float y,float z) {
		__pm_assign_c(*this, x,y,z);
	}
	/// Copy constructor.
	vec3(const vec3& vec) {
		__pm_assign_v(*this, vec);
	}
	/// Move constructor.
	vec3(const vec3&& vec) {
		__pm_assign_v(*this, vec);
	}
	/// Assignation operator.
	vec3& operator= (const vec3& vec) {
		__pm_assign_v(*this, vec); return *this;
	}
	/// Scalar-vector addition.
	inline vec3 operator+ (float k) const		{ vec3 r; __pm_add_v_s(r, *this, k);	return r; }
	/// Vector-vector addition.
	inline vec3 operator+ (const vec3& k) const	{ vec3 r; __pm_add_v_v(r, *this, k);	return r; }
	/// Scalar-vector addition.
	inline vec3& operator+= (float k)			{         __pm_add_acc_s( *this, k);	return *this; }
	/// Vector-vector addition.
	inline vec3& operator+= (const vec3& k)		{         __pm_add_acc_v( *this, k);	return *this; }
	/// Unary '-' operator. Inverts direction of vector.
	inline vec3 operator- () const				{ vec3 r; __pm_invert(r,*this);		return r; }
	/// Scalar-vector substraction.
	inline vec3 operator- (float k) const		{ vec3 r; __pm_sub_v_s(r, *this, k);	return r; }
	/// Vector-vector substraction.
	inline vec3 operator- (const vec3& k) const	{ vec3 r; __pm_sub_v_v(r, *this, k);	return r; }
	/// Scalar-vector substraction.
	inline vec3& operator-= (float k)			{         __pm_sub_acc_s( *this, k);	return *this; }
	/// Vector-vector substraction.
	inline vec3& operator-= (const vec3& k)		{         __pm_sub_acc_v( *this, k);	return *this; }
	/// Scalar-vector multiplication.
	inline vec3 operator* (float k) const		{ vec3 r; __pm_mul_v_s(r, *this, k);	return r; }
	/// Vector-vector multiplication.
	inline vec3 operator* (const vec3& k) const	{ vec3 r; __pm_mul_v_v(r, *this, k);	return r; }
	/// Scalar-vector multiplication.
	inline vec3& operator*= (float k)			{         __pm_mul_acc_s( *this, k);	return *this; }
	/// Vector-vector multiplication.
	inline vec3& operator*= (const vec3& k)		{         __pm_mul_acc_v( *this, k);	return *this; }
	/// X-coordinate value.
	inline float x() const { return __x; }
	/// Y-coordinate value.
	inline float y() const { return __y; }
	/// Z-coordinate value.
	inline float z() const { return __z; }
} vec3;

/* GEOMETRY */

// The expression of the dot product between two vectors.
#define __pm_dot(f,g)					\
	((f).__x*(g).__x +					\
	 (f).__y*(g).__y +					\
	 (f).__z*(g).__z)

// The expression for the square of the norm of a vector.
#define __pm_norm2(g)	(__pm_dot(g,g))
// The expression for the norm of a vector.
#define __pm_norm(g)	(std::sqrt((__pm_norm2(g))))

// The expression for the squared distance between
// two points, given their position vectors.
#define __pm_dist2(u,v)							\
	(((u).__x - (v).__x)*((u).__x - (v).__x) +	\
	 ((u).__y - (v).__y)*((u).__y - (v).__y) +	\
	 ((u).__z - (v).__z)*((u).__z - (v).__z))
// The expression for the distance between
// two points, given their position vectors.
#define __pm_dist(u,v) (std::sqrt(__pm_dist2(u,v)))

// The computation of the cross product of vectors 'f' and 'g'.
// Result is stored in 'h'. Do not use this macro as
// '__pm_cross(f, f,g)' since the result will not be correct.
#define __pm_cross(h, f,g)								\
	__pm_assign_c(h, (f).__y*(g).__z - (f).__z*(g).__y,	\
					 (f).__z*(g).__x - (f).__x*(g).__z,	\
					 (f).__x*(g).__y - (f).__y*(g).__x)

// Make a perpendicular vector to 'u'.
#define __pm_perp(v, u)			\
	__pm_assign_c(v,(u).__y,	\
					-(u).__x,	\
					0.0f)

/**
 * @brief Vector normalisation.
 *
 * Each component of vector @e in is divided by its norm. The result
 * is stored in @e out.
 * @param[in] in Vector to be normalised.
 * @param[out] out Where to store the normalised vector.
 */
inline void normalise_vec3(const vec3& in, vec3& out) {
	float norm = __pm_norm(in);
	__pm_div_v_s(out, in, norm);
}
// Normalise vector 'g'.
#define __pm_normalise(f,g)	normalise_vec3(g,f)

} // -- namespace math
} // -- namespace physim
