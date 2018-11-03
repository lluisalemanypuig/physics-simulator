#pragma once

// C++ includes
#include <cmath>

namespace physim {
namespace math {

/**
 * @brief 4-dimensional vector.
 *
 * Basic implementation of a four-dimensional vector as a struct
 * with only four attributes @ref vec4::__x, @ref vec4::__y,
 * @ref vec4::__z and @ref vec4::__w.
 *
 * The notation is borrowed from the 'glm' library.
 */
typedef struct vec4 {
	/// First component of the vector.
	float x;
	/// Second component of the vector.
	float y;
	/// Third component of the vector.
	float z;
	/// Fourth component of the vector.
	float w;

	/// Default constructor.
	vec4()										{ x = y = z = w = 0.0f; }
	/// Constructor at point (@e s, @e s, @e s, @e s).
	vec4(float s)								{ x = y = z = w = s; }
	/// Construct a vector with coordinates (@e _x, @e _y, @e _z, @e _w).
	vec4(float _x,float _y,float _z,float _w)	{ x = _x; y = _y; z = _z; w = _w; }
	/// Copy constructor.
	vec4(const vec4& v)							{ x = v.x; y = v.y; z = v.z; w = v.w; }
	/// Assignation operator.
	vec4& operator= (const vec4& v)				{ x = v.x; y = v.y; z = v.z; w = v.w;									return *this; }
	/// Scalar-vector addition.
	inline vec4 operator+ (float s) const		{ vec4 r;	r.x = x + s; r.y = y + s; r.z = z + s; r.w = w + s;			return r; }
	/// Vector-vector addition.
	inline vec4 operator+ (const vec4& v) const	{ vec4 r;	r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; r.w = w + v.w;	return r; }
	/// Scalar-vector addition.
	inline vec4& operator+= (float s)			{			x += s; y += s; z += s; w += s;								return *this; }
	/// Vector-vector addition.
	inline vec4& operator+= (const vec4& v)		{			x += v.x; y += v.y; z += v.z; w += v.w;						return *this; }
	/// Unary '-' operator. Inverts direction of vector.
	inline vec4 operator- () const				{ vec4 r;	r.x = -x; r.y = -y; r.z = -z; r.w = -w;						return r; }
	/// Scalar-vector substraction.
	inline vec4 operator- (float s) const		{ vec4 r;	r.x = x - s; r.y = y - s; r.z = z - s; r.w = w - s;			return r; }
	/// Vector-vector substraction.
	inline vec4 operator- (const vec4& v) const	{ vec4 r;	r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; w.z = w - v.w;	return r; }
	/// Scalar-vector substraction.
	inline vec4& operator-= (float s)			{			x -= s; y -= s; z -= s; w -= s;								return *this; }
	/// Vector-vector substraction.
	inline vec4& operator-= (const vec4& v)		{			x -= v.x; y -= v.y; z -= v.z; w -= v.w;						return *this; }
	/// Scalar-vector multiplication.
	inline vec4 operator* (float k) const		{ vec4 r;	r.x = x*k; r.y = y*k; r.z = z*k; r.w = w*k;					return r; }
	/// Vector-vector multiplication.
	inline vec4 operator* (const vec4& v) const	{ vec4 r;	r.x = x*v.x; r.y = y*v.y; r.z = z*v.z; r.w = w*v.w;			return r; }
	/// Scalar-vector multiplication.
	inline vec4& operator*= (float s)			{			x *= s; y *= s; z *= s; w *= s;								return *this; }
	/// Vector-vector multiplication.
	inline vec4& operator*= (const vec4& v)		{			x *= v.x; y *= v.y; z *= v.z; w *= v.w;						return *this; }
} vec4;

/* GEOMETRY */

/// The dot product between two vectors.
inline float dot(const vec4& f, const vec4& g)	{ return f.x*g.x + f.y*g.y + f.z*g.z + f.w*g.w; }

/// The square of the norm of a vector.
inline float norm2(const vec4& f) { return dot(f,f); }
/// The norm of a vector.
inline float norm(const vec4& f) { return std::sqrt(dot(f,f)); }

/// The squared distance between two points, given their positional vectors.
inline float dist2(const vec4& f, const vec4& g) {
	return (f.x - g.x)*(f.x - g.x) +
		   (f.y - g.y)*(f.y - g.y) +
		   (f.z - g.z)*(f.z - g.z) +
		   (f.w - g.w)*(f.w - g.w);
}
/// The distance between two points, given their positional vectors.
inline float dist(const vec4& f, const vec4& g) {
	return std::sqrt(dist2(f,g));
}

/**
 * @brief Makes a perpendicular vector to @e f.
 *
 * Two vectors are perpendicular if, and only if, their
 * dot product equals 0.
 * @param[in] f Input vector.
 * @param[out] g A vector perpendicular to @e f.
 */
inline void perpendicular(const vec4& f, vec4& g) {
	g.x = f.y;
	g.y = -f.x;
	g.z = 0.0f;
	g.w = 0.0f;
}
/**
 * @brief Makes a perpendicular vector to @e f.
 *
 * Two vectors are perpendicular if, and only if, their
 * dot product equals 0.
 * @param f Input vector.
 * @returns Returns a vector perpendicular to @e f.
 */
inline vec4 perpendicular(const vec4& f) { vec4 g; perpendicular(f,g); return g; }

/**
 * @brief Vector normalisation.
 *
 * Each component of vector @e f is divided by its norm.
 * The result is stored in @e g.
 * @param[in] f Vector to be normalised.
 * @param[out] g Where to store the normalised vector.
 */
inline void normalise(const vec4& f, vec4& g) {
	float n = norm(f);
	g.x = f.x*(1.0f/n);
	g.y = f.y*(1.0f/n);
	g.z = f.z*(1.0f/n);
	g.w = f.w*(1.0f/n);
}
/**
 * @brief Vector normalisation.
 *
 * Each component of vector @e f is divided by its norm.
 * @param f Vector to be normalised.
 * @returns Returns the normalisation of vector @e f.
 */
inline vec4 normalise(const vec4& f) {
	vec4 out;
	normalise(f, out);
	return out;
}

} // -- namespace math
} // -- namespace physim
