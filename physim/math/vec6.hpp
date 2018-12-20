#pragma once

// C++ includes
#include <cmath>

// physim includes
#include <physim/math/vec_templates.hpp>

namespace physim {
namespace math {

/**
 * @brief 6-dimensional vector.
 *
 * Basic implementation of a six-dimensional vector as a struct
 * with only six attributes @ref vec6::x, @ref vec6::y,
 * @ref vec6::z, @ref vec6::u, @ref vec6::v and @ref vec6::w.
 *
 * The notation is borrowed from the 'glm' library.
 */
typedef struct vec6 {
	/// First component of the vector.
	float x;
	/// Second component of the vector.
	float y;
	/// Third component of the vector.
	float z;
	/// Fourth component of the vector.
	float u;
	/// Fifth component of the vector.
	float v;
	/// Sixth component of the vector.
	float w;

	/// Default constructor.
	vec6()										{ x = y = z = u = v = w = 0.0f; }
	/// Constructor at point (@e s, @e s, @e s, @e s).
	vec6(float s)								{ x = y = z = u = v = w = s; }
	/// Construct a vector with coordinates (@e _x, @e _y, @e _z, @e _w).
	vec6(float _x,float _y,float _z,float _w,float _u,float _v)	{ x = _x; y = _y; z = _z; u = _u; v = _v; w = _w; }
	/// Copy constructor.
	vec6(const vec6& p)							{			x = p.x; y = p.y; z = p.z; u = p.u; v = p.v; w = p.w; }
	/// Assignation operator.
	vec6& operator= (const vec6& p)				{			x = p.x; y = p.y; z = p.z; u = p.u; v = p.v; w = p.w; return *this; }
	/// Vector-scalar addition.
	inline vec6 operator+ (float s) const		{ vec6 r;	r.x = x + s; r.y = y + s; r.z = z + s; r.u = u + s; r.v = v + s; r.w = w + s; return r; }
	/// Vector-vector addition.
	inline vec6 operator+ (const vec6& p) const	{ vec6 r;	r.x = x + p.x; r.y = y + p.y; r.z = z + p.z; r.u = u + p.u; r.v = v + p.v; r.w = w + p.w; return r; }
	/// Vector-scalar addition.
	inline vec6& operator+= (float s)			{			x += s; y += s; z += s; u += s; v += s; w += s; return *this; }
	/// Vector-vector addition.
	inline vec6& operator+= (const vec6& p)		{			x += p.x; y += p.y; z += p.z; u += p.u; v += p.v; w += p.w; return *this; }
	/// Unary '-' operator. Inverts direction of vector.
	inline vec6 operator- () const				{ vec6 r;	r.x = -x; r.y = -y; r.z = -z; r.u = -u; r.v = -v; r.w = -w; return r; }
	/// Vector-scalar substraction.
	inline vec6 operator- (float s) const		{ vec6 r;	r.x = x - s; r.y = y - s; r.z = z - s; r.u = u - s; r.v = v - s; r.w = w - s; return r; }
	/// Vector-vector substraction.
	inline vec6 operator- (const vec6& p) const	{ vec6 r;	r.x = x - p.x; r.y = y - p.y; r.z = z - p.z; r.u = u - p.u; r.v = v - p.v; r.w = w - p.w; return r; }
	/// Vector-scalar substraction.
	inline vec6& operator-= (float s)			{			x -= s; y -= s; z -= s; u -= s; v -= s; w -= s; return *this; }
	/// Vector-vector substraction.
	inline vec6& operator-= (const vec6& p)		{			x -= p.x; y -= p.y; z -= p.z; u -= p.u; v -= p.v; w -= p.w; return *this; }
	/// Vector-scalar multiplication.
	inline vec6 operator* (float k) const		{ vec6 r;	r.x = x*k; r.y = y*k; r.z = z*k; r.u = u*k; r.v = v*k; r.w = w*k; return r; }
	/// Vector-vector multiplication.
	inline vec6 operator* (const vec6& p) const	{ vec6 r;	r.x = x*p.x; r.y = y*p.y; r.z = z*p.z; r.u = u*p.u; r.v = v*p.v; r.w = w*p.w; return r; }
	/// Vector-scalar multiplication.
	inline vec6& operator*= (float s)			{			x *= s; y *= s; z *= s; u *= s; v *= s; w *= s; return *this; }
	/// Vector-vector multiplication.
	inline vec6& operator*= (const vec6& p)		{			x *= p.x; y *= p.y; z *= p.z; u *= p.u; v *= p.v; w *= p.w; return *this; }
	/// Vector-scalar division.
	inline vec6 operator/ (float k) const		{ vec6 r;	r.x = x*(1.0f/k); r.y = y*(1.0f/k); r.z = z*(1.0f/k); r.u = u*(1.0f/k); r.v = v*(1.0f/k); r.w = w*(1.0f/k); return r; }
	/// Vector-vector division.
	inline vec6 operator/ (const vec6& p) const	{ vec6 r;	r.x = x*(1.0f/p.x);	r.y = y*(1.0f/p.y); r.z = z*(1.0f/p.z); r.u = u*(1.0f/p.u); r.v = v*(1.0f/p.v); r.w = w*(1.0f/p.w);	return r; }
	/// Vector-scalar division.
	inline vec6& operator/= (float s)			{			x *= (1.0f/s); y *= (1.0f/s); z *= (1.0f/s); u *= (1.0f/s); v *= (1.0f/s); w *= (1.0f/s); return *this; }
	/// Vector-vector division.
	inline vec6& operator/= (const vec6& p)		{			x *= (1.0f/p.x); y *= (1.0f/p.y); z *= (1.0f/p.z); u *= (1.0f/p.u); v *= (1.0f/p.v); w *= (1.0f/p.w); return *this; }
} vec6;

/* MISCELLANEA */

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
static inline void min(const vec6& a, const vec6& b, vec6& m) {
	m.x = (a.x < b.x ? a.x : b.x);
	m.y = (a.y < b.y ? a.y : b.y);
	m.z = (a.z < b.z ? a.z : b.z);
	m.u = (a.u < b.u ? a.u : b.u);
	m.v = (a.v < b.v ? a.v : b.v);
	m.w = (a.w < b.w ? a.w : b.w);
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
static inline void max(const vec6& a, const vec6& b, vec6& M) {
	M.x = (a.x > b.x ? a.x : b.x);
	M.y = (a.y > b.y ? a.y : b.y);
	M.z = (a.z > b.z ? a.z : b.z);
	M.u = (a.u > b.u ? a.u : b.u);
	M.v = (a.v > b.v ? a.v : b.v);
	M.w = (a.w > b.w ? a.w : b.w);
}

/* GEOMETRY */

/// The dot product between two vectors.
static inline float dot(const vec6& f, const vec6& g)
{ return f.x*g.x + f.y*g.y + f.z*g.z + f.u*g.u + f.v*g.v + f.w*g.w; }

/// The square of the norm of a vector.
static inline float norm2(const vec6& f) { return dot(f,f); }
/// The norm of a vector.
static inline float norm(const vec6& f) { return std::sqrt(dot(f,f)); }

/// The squared distance between two points, given their positional vectors.
static inline float dist2(const vec6& f, const vec6& g) {
	return (f.x - g.x)*(f.x - g.x) +
		   (f.y - g.y)*(f.y - g.y) +
		   (f.z - g.z)*(f.z - g.z) +
		   (f.u - g.u)*(f.u - g.u) +
		   (f.v - g.v)*(f.v - g.v) +
		   (f.w - g.w)*(f.w - g.w);
}

/**
 * @brief Vector normalisation.
 *
 * Each component of vector @e f is divided by its norm.
 * The result is stored in @e g.
 * @param[in] f Vector to be normalised.
 * @param[out] g Where to store the normalised vector.
 */
static inline void normalise(const vec6& f, vec6& g) {
	float n = norm(f);
	g.x = f.x*(1.0f/n);
	g.y = f.y*(1.0f/n);
	g.z = f.z*(1.0f/n);
	g.u = f.u*(1.0f/n);
	g.v = f.v*(1.0f/n);
	g.w = f.w*(1.0f/n);
}

} // -- namespace math
} // -- namespace physim
