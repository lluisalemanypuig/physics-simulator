#pragma once

// C++ includes
#include <fstream>
#include <cmath>

// physim includes
#include <physim/math/math_ops.hpp>
#include <physim/math/math_misc.hpp>

namespace physim {
namespace math {

/// Definition of 3-dimensional vector.
typedef struct vec3 {
	float __x, __y, __z;

	vec3() { __pm_assign_c(*this, 0.0f,0.0f,0.0f); }
	vec3(float s) {
		__pm_assign_s(*this, s);
	}
	vec3(float x,float y,float z) {
		__pm_assign_c(*this, x,y,z);
	}
	vec3(const vec3& vec) {
		__pm_assign_v(*this, vec);
	}
	vec3(const vec3&& vec) {
		__pm_assign_v(*this, vec);
	}
	vec3& operator= (const vec3& vec) {
		__pm_assign_v(*this, vec); return *this;
	}
	inline friend std::ostream& operator<< (std::ostream& os, const vec3& k) {
		os << k.x() << "," << k.y() << "," << k.z();
		return os;
	}
	inline vec3 operator+ (float k) const		{ vec3 r; __pm_add_v_s(r, *this, k);	return r; }
	inline vec3 operator+ (const vec3& k) const	{ vec3 r; __pm_add_v_v(r, *this, k);	return r; }
	inline vec3& operator+= (float k)			{         __pm_add_acc_s( *this, k);	return *this; }
	inline vec3& operator+= (const vec3& k)		{         __pm_add_acc_v( *this, k);	return *this; }
	inline vec3 operator- () const				{ vec3 r; __pm_conjugate(r,*this);		return r; }
	inline vec3 operator- (float k) const		{ vec3 r; __pm_sub_v_s(r, *this, k);	return r; }
	inline vec3 operator- (const vec3& k) const	{ vec3 r; __pm_sub_v_v(r, *this, k);	return r; }
	inline vec3& operator-= (float k)			{         __pm_sub_acc_s( *this, k);	return *this; }
	inline vec3& operator-= (const vec3& k)		{         __pm_sub_acc_v( *this, k);	return *this; }
	inline vec3 operator* (float k) const		{ vec3 r; __pm_mul_v_s(r, *this, k);	return r; }
	inline vec3 operator* (const vec3& k) const	{ vec3 r; __pm_mul_v_v(r, *this, k);	return r; }
	inline vec3& operator*= (float k)			{         __pm_mul_acc_s( *this, k);	return *this; }
	inline vec3& operator*= (const vec3& k)		{         __pm_mul_acc_v( *this, k);	return *this; }
	inline float x() const { return __x; }
	inline float y() const { return __y; }
	inline float z() const { return __z; }
} vec3;

/* GEOMETRY */

#define __pm_dot(f,g)					\
	((f).__x*(g).__x +					\
	 (f).__y*(g).__y +					\
	 (f).__z*(g).__z)

#define __pm_norm2(g)	(__pm_dot(g,g))
#define __pm_norm(g)	(std::sqrt((__pm_norm2(g))))

#define __pm_dist2(u,v)							\
	(((u).__x - (v).__x)*((u).__x - (v).__x) +	\
	 ((u).__y - (v).__y)*((u).__y - (v).__y) +	\
	 ((u).__z - (v).__z)*((u).__z - (v).__z))
#define __pm_dist(u,v) (std::sqrt(__pm_dist2(u,v)))

#define __pm_cross(h, f,g)								\
	__pm_assign_c(h, (f).__y*(g).__z - (f).__z*(g).__y,	\
					 (f).__z*(g).__x - (f).__x*(g).__z,	\
					 (f).__x*(g).__y - (f).__y*(g).__x)

#define __pm_perp(v, u)			\
	__pm_assign_c(v,(u).__y,	\
					-(u).__x,	\
					0.0f)

inline void normalise_vec3(const vec3& in, vec3& out) {
	float norm = __pm_norm(in);
	__pm_div_v_s(out, in, norm);
}
#define __pm_normalise(f,g)	normalise_vec3(g,f)

} // -- namespace math
} // -- namespace physim
