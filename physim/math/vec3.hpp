#pragma once

// C++ includes
#include <cmath>

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
	float x;
	/// Second component of the vector.
	float y;
	/// Third component of the vector.
	float z;

	/// Default constructor.
	vec3()								{ x = y = z = 0.0f; }
	/// Constructor at point (@e s, @e s, @e s).
	vec3(float s)						{ x = y = z = s; }
	/// Construct a vector with coordinates (@e _x, @e _y, @e _z).
	vec3(float _x,float _y,float _z)	{ x = _x; y = _y; z = _z; }
	/// Copy constructor.
	vec3(const vec3& v)					{ x = v.x; y = v.y; z = v.z; }
	/// Move constructor.
	vec3(const vec3&& v)				{ x = v.x; y = v.y; z = v.z; }
	/// Assignation operator.
	vec3& operator= (const vec3& v)		{ x = v.x; y = v.y; z = v.z;	return *this; }
	/// Scalar-vector addition.
	inline vec3 operator+ (float s) const
	{ vec3 r;	r.x = x + s; r.y = y + s; r.z = z + s;			return r; }
	/// Vector-vector addition.
	inline vec3 operator+ (const vec3& v) const
	{ vec3 r;	r.x = x + v.x; r.y = y + v.y; r.z = z + v.z;	return r; }
	/// Scalar-vector addition.
	inline vec3& operator+= (float s)
	{			x += s; y += s; z += s;							return *this; }
	/// Vector-vector addition.
	inline vec3& operator+= (const vec3& v)
	{			x += v.x; y += v.y; z += v.z;					return *this; }
	/// Unary '-' operator. Inverts direction of vector.
	inline vec3 operator- () const
	{ vec3 r;	r.x = -x; r.y = -y; r.z = -z;					return r; }
	/// Scalar-vector substraction.
	inline vec3 operator- (float s) const
	{ vec3 r;	r.x = x - s; r.y = y - s; r.z = z - s;			return r; }
	/// Vector-vector substraction.
	inline vec3 operator- (const vec3& v) const
	{ vec3 r;	r.x = x - v.x; r.y = y - v.y; r.z = z - v.z;	return r; }
	/// Scalar-vector substraction.
	inline vec3& operator-= (float s)
	{			x -= s; y -= s; z -= s;							return *this; }
	/// Vector-vector substraction.
	inline vec3& operator-= (const vec3& v)
	{			x -= v.x; y -= v.y; z -= v.z;					return *this; }
	/// Scalar-vector multiplication.
	inline vec3 operator* (float k) const
	{ vec3 r;	r.x = x*k; r.y = y*k; r.z = z*k;				return r; }
	/// Vector-vector multiplication.
	inline vec3 operator* (const vec3& v) const
	{ vec3 r;	r.x = x*v.x; r.y = y*v.y; r.z = z*v.z;			return r; }
	/// Scalar-vector multiplication.
	inline vec3& operator*= (float s)
	{			x *= s; y *= s; z *= s;							return *this; }
	/// Vector-vector multiplication.
	inline vec3& operator*= (const vec3& v)
	{			x *= v.x; y *= v.y; z *= v.z;					return *this; }
} vec3;

/* GEOMETRY */

/// The dot product between two vectors.
inline float dot(const vec3& f, const vec3& g) { return f.x*g.x + f.y*g.y + f.z*g.z; }

/// The square of the norm of a vector.
inline float norm2(const vec3& f) { return dot(f,f); }
/// The norm of a vector.
inline float norm(const vec3& f) { return std::sqrt(dot(f,f)); }

/// The squared distance between two points, given their positional vectors.
inline float dist2(const vec3& f, const vec3& g) {
	return (f.x - g.x)*(f.x - g.x) +
		   (f.y - g.y)*(f.y - g.y) +
		   (f.z - g.z)*(f.z - g.z);
}
/// The distance between two points, given their positional vectors.
inline float dist(const vec3& f, const vec3& g) {
	return std::sqrt(dist2(f,g));
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
inline vec3 perpendicular(const vec3& f) { vec3 g; perpendicular(f,g); return g; }

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
 * @brief Vector normalisation.
 *
 * Each component of vector @e f is divided by its norm.
 * @param f Vector to be normalised.
 * @returns Returns the normalisation of vector @e f.
 */
inline vec3 normalise(const vec3& f) {
	vec3 out;
	float n = norm(f);
	out.x = f.x*(1.0f/n);
	out.y = f.y*(1.0f/n);
	out.z = f.z*(1.0f/n);
	return out;
}

} // -- namespace math
} // -- namespace physim
