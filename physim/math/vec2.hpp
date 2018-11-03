#pragma once

// C++ includes
#include <cmath>

namespace physim {
namespace math {

/**
 * @brief 2-dimensional vector.
 *
 * Basic implementation of a two-dimensional vector as a struct
 * with only two attributes @ref vec2::__x, @ref vec2::__y.
 *
 * The notation is borrowed from the 'glm' library.
 */
typedef struct vec2 {
	/// First component of the vector.
	float x;
	/// Second component of the vector.
	float y;

	/// Default constructor.
	vec2()										{ x = y = 0.0f; }
	/// Constructor at point (@e s, @e s).
	vec2(float s)								{ x = y = s; }
	/// Construct a vector with coordinates (@e _x, @e _y).
	vec2(float _x,float _y)			{ x = _x; y = _y; }
	/// Copy constructor.
	vec2(const vec2& v)							{ x = v.x; y = v.y; }
	/// Assignation operator.
	vec2& operator= (const vec2& v)				{ x = v.x; y = v.y;							return *this; }
	/// Scalar-vector addition.
	inline vec2 operator+ (float s) const		{ vec2 r;	r.x = x + s; r.y = y + s;		return r; }
	/// Vector-vector addition.
	inline vec2 operator+ (const vec2& v) const	{ vec2 r;	r.x = x + v.x; r.y = y + v.y;	return r; }
	/// Scalar-vector addition.
	inline vec2& operator+= (float s)			{			x += s; y += s;					return *this; }
	/// Vector-vector addition.
	inline vec2& operator+= (const vec2& v)		{			x += v.x; y += v.y;				return *this; }
	/// Unary '-' operator. Inverts direction of vector.
	inline vec2 operator- () const				{ vec2 r;	r.x = -x; r.y = -y;				return r; }
	/// Scalar-vector substraction.
	inline vec2 operator- (float s) const		{ vec2 r;	r.x = x - s; r.y = y - s;		return r; }
	/// Vector-vector substraction.
	inline vec2 operator- (const vec2& v) const	{ vec2 r;	r.x = x - v.x; r.y = y - v.y;	return r; }
	/// Scalar-vector substraction.
	inline vec2& operator-= (float s)			{			x -= s; y -= s;					return *this; }
	/// Vector-vector substraction.
	inline vec2& operator-= (const vec2& v)		{			x -= v.x; y -= v.y;				return *this; }
	/// Scalar-vector multiplication.
	inline vec2 operator* (float k) const		{ vec2 r;	r.x = x*k; r.y = y*k;			return r; }
	/// Vector-vector multiplication.
	inline vec2 operator* (const vec2& v) const	{ vec2 r;	r.x = x*v.x; r.y = y*v.y;		return r; }
	/// Scalar-vector multiplication.
	inline vec2& operator*= (float s)			{			x *= s; y *= s;					return *this; }
	/// Vector-vector multiplication.
	inline vec2& operator*= (const vec2& v)		{			x *= v.x; y *= v.y;				return *this; }
} vec2;

/* GEOMETRY */

/// The dot product between two vectors.
inline float dot(const vec2& f, const vec2& g)	{ return f.x*g.x + f.y*g.y; }

/// The square of the norm of a vector.
inline float norm2(const vec2& f) { return dot(f,f); }
/// The norm of a vector.
inline float norm(const vec2& f) { return std::sqrt(dot(f,f)); }

/// The squared distance between two points, given their positional vectors.
inline float dist2(const vec2& f, const vec2& g) {
	return (f.x - g.x)*(f.x - g.x) +
		   (f.y - g.y)*(f.y - g.y);
}
/// The distance between two points, given their positional vectors.
inline float dist(const vec2& f, const vec2& g) {
	return std::sqrt(dist2(f,g));
}

/**
 * @brief Makes a perpendicular vector to @e f.
 *
 * Two vectors are perpendicular if, and only if, their
 * dot product equals 0.
 * @param[out] g A vector perpendicular to @e f.
 * @param[in] f Input vector.
 */
inline void perpendicular(const vec2& f, vec2& g) {
	g.x = f.y;
	g.y = -f.x;
}
/**
 * @brief Makes a perpendicular vector to @e f.
 *
 * Two vectors are perpendicular if, and only if, their
 * dot product equals 0.
 * @param f Input vector.
 * @returns Returns a vector perpendicular to @e f.
 */
inline vec2 perpendicular(const vec2& f) { vec2 g; perpendicular(f,g); return g; }

/**
 * @brief Vector normalisation.
 *
 * Each component of vector @e f is divided by its norm.
 * The result is stored in @e g.
 * @param[in] f Vector to be normalised.
 * @param[out] g Where to store the normalised vector.
 */
inline void normalise(const vec2& f, vec2& g) {
	float n = norm(f);
	g.x = f.x*(1.0f/n);
	g.y = f.y*(1.0f/n);
}
/**
 * @brief Vector normalisation.
 *
 * Each component of vector @e f is divided by its norm.
 * @param f Vector to be normalised.
 * @returns Returns the normalisation of vector @e f.
 */
inline vec2 normalise(const vec2& f) {
	vec2 out;
	normalise(f, out);
	return out;
}

} // -- namespace math
} // -- namespace physim
