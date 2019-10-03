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

// C++ includes
#include <cmath>

// physim includes
#include <physim/math/vec2.hpp>
#include <physim/math/vec_templates.hpp>

namespace physim {
namespace math {

/**
 * @brief 3-dimensional vector.
 *
 * Basic implementation of a three-dimensional vector as a struct
 * with only three attributes @ref vec3::x, @ref vec3::y and @ref vec3::z.
 *
 * The notation is borrowed from the 'glm' library.
 */
typedef struct vec3 {
	/// First component of the vector.
	float x;
	/// Second component of the vector.
	float y;
	/// Third component of the vector.
	float z;

	/// Default constructor.
	vec3()										{ x = y = z = 0.0f; }
	/// Constructor at point (@e s, @e s, @e s).
	vec3(float s)								{ x = y = z = s; }
	/// Construct a vector with coordinates (@e _x, @e _y, @e _z).
	vec3(float _x,float _y,float _z)			{ x = _x; y = _y; z = _z; }
	/// Construct a vector with @ref vec2 and a coordinate.
	vec3(const vec2& p,float _z)				{ x = p.x; y = p.y; z = _z; }
	/// Copy constructor.
	vec3(const vec3& p)							{ x = p.x; y = p.y; z = p.z; }
	/// Assignation operator.
	vec3& operator= (const vec3& p)				{ x = p.x; y = p.y; z = p.z;	return *this; }
	/// Vector-scalar addition.
	inline vec3 operator+ (float s) const		{ vec3 r;	r.x = x + s; r.y = y + s; r.z = z + s;			return r; }
	/// Vector-vector addition.
	inline vec3 operator+ (const vec3& p) const	{ vec3 r;	r.x = x + p.x; r.y = y + p.y; r.z = z + p.z;	return r; }
	/// Vector-scalar addition.
	inline vec3& operator+= (float s)			{			x += s; y += s; z += s;							return *this; }
	/// Vector-vector addition.
	inline vec3& operator+= (const vec3& p)		{			x += p.x; y += p.y; z += p.z;					return *this; }
	/// Unary '-' operator. Inverts direction of vector.
	inline vec3 operator- () const				{ vec3 r;	r.x = -x; r.y = -y; r.z = -z;					return r; }
	/// Vector-scalar substraction.
	inline vec3 operator- (float s) const		{ vec3 r;	r.x = x - s; r.y = y - s; r.z = z - s;			return r; }
	/// Vector-vector substraction.
	inline vec3 operator- (const vec3& p) const	{ vec3 r;	r.x = x - p.x; r.y = y - p.y; r.z = z - p.z;	return r; }
	/// Vector-scalar substraction.
	inline vec3& operator-= (float s)			{			x -= s; y -= s; z -= s;							return *this; }
	/// Vector-vector substraction.
	inline vec3& operator-= (const vec3& p)		{			x -= p.x; y -= p.y; z -= p.z;					return *this; }
	/// Vector-scalar multiplication.
	inline vec3 operator* (float k) const		{ vec3 r;	r.x = x*k; r.y = y*k; r.z = z*k;				return r; }
	/// Vector-vector multiplication.
	inline vec3 operator* (const vec3& p) const	{ vec3 r;	r.x = x*p.x; r.y = y*p.y; r.z = z*p.z;			return r; }
	/// Vector-scalar multiplication.
	inline vec3& operator*= (float s)			{			x *= s; y *= s; z *= s;							return *this; }
	/// Vector-vector multiplication.
	inline vec3& operator*= (const vec3& p)		{			x *= p.x; y *= p.y; z *= p.z;					return *this; }
	/// Vector-scalar division.
	inline vec3 operator/ (float k) const		{ vec3 r;	r.x = x*(1.0f/k); r.y = y*(1.0f/k); r.z = z*(1.0f/k);		return r; }
	/// Vector-vector division.
	inline vec3 operator/ (const vec3& p) const	{ vec3 r;	r.x = x*(1.0f/p.x); r.y = y*(1.0f/p.y); r.z = z*(1.0f/p.z);	return r; }
	/// Vector-scalar division.
	inline vec3& operator/= (float s)			{			x *= (1.0f/s); y *= (1.0f/s); z *= (1.0f/s);				return *this; }
	/// Vector-vector division.
	inline vec3& operator/= (const vec3& p)		{			x *= (1.0f/p.x); y *= (1.0f/p.y); z *= (1.0f/p.z);			return *this; }
} vec3;

/**
 * @brief Computes the minimum of two vectors.
 *
 * The first component of the result is the minimum value of the
 * first components of the inputs, the second component of the result
 * is the minimum value of the second components of the inputs, ...
 * @param[in] a Input vector.
 * @param[in] b Input vector.
 * @param[out] m Minimum of @e a and @e b.
 */
inline void min(const vec3& a, const vec3& b, vec3& m) {
	m.x = (a.x < b.x ? a.x : b.x);
	m.y = (a.y < b.y ? a.y : b.y);
	m.z = (a.z < b.z ? a.z : b.z);
}

/**
 * @brief Computes the maximum of two vectors.
 *
 * The first component of the result is the maximum value of the
 * first components of the inputs, the second component of the result
 * is the maximum value of the second components of the inputs, ...
 * @param[in] a Input vector.
 * @param[in] b Input vector.
 * @param[out] M Maximum of @e a and @e b.
 */
inline void max(const vec3& a, const vec3& b, vec3& M) {
	M.x = (a.x > b.x ? a.x : b.x);
	M.y = (a.y > b.y ? a.y : b.y);
	M.z = (a.z > b.z ? a.z : b.z);
}

/* GEOMETRY */

/// The dot product between two vectors.
inline float dot(const vec3& f, const vec3& g)	{ return f.x*g.x + f.y*g.y + f.z*g.z; }

/// The squared distance between two points, given their positional vectors.
inline float dist2(const vec3& f, const vec3& g) {
	return (f.x - g.x)*(f.x - g.x) +
		   (f.y - g.y)*(f.y - g.y) +
		   (f.z - g.z)*(f.z - g.z);
}

/**
 * @brief The cross product of two vectors.
 * @param[out] h The cross product of @e f and @e g.
 * @param[in] f Input vector.
 * @param[in] g Input vector.
 */
inline void cross(const vec3& f, const vec3& g, vec3& h) {
	h.x = f.y*g.z - f.z*g.y;
	h.y = f.z*g.x - f.x*g.z;
	h.z = f.x*g.y - f.y*g.x;
}
/**
 * @brief The cross product of two vectors.
 * @param[in] f Input vector.
 * @param[in] g Input vector.
 * @returns Returns the cross product of @e f and @e g.
 */
inline vec3 cross(const vec3& f, const vec3& g) { vec3 h; cross(f,g,h); return h; }

/**
 * @brief Makes a perpendicular vector to @e f.
 *
 * Two vectors are perpendicular if, and only if, their
 * dot product equals 0.
 * @param[out] g A vector perpendicular to @e f.
 * @param[in] f Input vector.
 */
inline void perpendicular(const vec3& f, vec3& g) {
	g.x = f.y;
	g.y = -f.x;
	g.z = 0.0f;
}
/**
 * @brief Makes a perpendicular vector to @e f.
 *
 * Two vectors are perpendicular if, and only if, their
 * dot product equals 0.
 * @param f Input vector.
 * @returns Returns a vector perpendicular to @e f.
 */
inline vec3 perpendicular(const vec3& f) {
	vec3 g; perpendicular(f,g); return g;
}

/**
 * @brief Vector normalisation.
 *
 * Each component of vector @e f is divided by its norm.
 * The result is stored in @e g.
 * @param[in] f Vector to be normalised.
 * @param[out] g Where to store the normalised vector.
 */
inline void normalise(const vec3& f, vec3& g) {
	float n = norm(f);
	g.x = f.x*(1.0f/n);
	g.y = f.y*(1.0f/n);
	g.z = f.z*(1.0f/n);
}

/**
 * @brief Returns the angle between two vectors.
 * @param[in] f 3D vector.
 * @param[in] g 3D vector.
 * @return Returns the angle between the vectors \f$f,g\f$.
 */
inline float angle_xyz(const vec3& f, const vec3& g) {
	float frac = dot(f,g)/(norm(f)*norm(g));
	// truncate the value of frag so that we don't gent NaNs
	frac = (frac <= -1.0f)*(-1.0f) +
		   (frac >= 1.0f)*(1.0f) +
		   (-1.0f < frac and frac < 1.0f)*frac;
	return std::acos(frac);
}

/**
 * @brief Returns the xy angle between two vectors.
 * @param[in] f 3D vector.
 * @param[in] g 3D vector.
 * @return Returns the angle between the vectors \f$(f_x,0,0)\f$
 * and \f$(g_x,g_y,0)\f$.
 */
inline float angle_xy(const vec3& f, const vec3& g) {
	vec3 fx(f.x,0.0f,0.0f);
	vec3 gxy(g.x,g.y,0.0f);
	return angle_xyz(fx,gxy);
}

/**
 * @brief Returns the xz angle between two vectors.
 * @param[in] f 3D vector.
 * @param[in] g 3D vector.
 * @return Returns the angle between the vectors \f$(f_x,f_y,0)\f$
 * and \f$g\f$.
 */
inline float angle_xz(const vec3& f, const vec3& g) {
	vec3 fxy(f.x,f.y,0.0f);
	return angle_xyz(fxy,g);
}

} // -- namespace math
} // -- namespace physim
