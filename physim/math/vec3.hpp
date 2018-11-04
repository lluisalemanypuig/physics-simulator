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
	vec3()										{ x = y = z = 0.0f; }
	/// Constructor at point (@e s, @e s, @e s).
	vec3(float s)								{ x = y = z = s; }
	/// Construct a vector with coordinates (@e _x, @e _y, @e _z).
	vec3(float _x,float _y,float _z)			{ x = _x; y = _y; z = _z; }
	/// Copy constructor.
	vec3(const vec3& v)							{ x = v.x; y = v.y; z = v.z; }
	/// Assignation operator.
	vec3& operator= (const vec3& v)				{ x = v.x; y = v.y; z = v.z;	return *this; }
	/// Vector-scalar addition.
	inline vec3 operator+ (float s) const		{ vec3 r;	r.x = x + s; r.y = y + s; r.z = z + s;			return r; }
	/// Vector-vector addition.
	inline vec3 operator+ (const vec3& v) const	{ vec3 r;	r.x = x + v.x; r.y = y + v.y; r.z = z + v.z;	return r; }
	/// Vector-scalar addition.
	inline vec3& operator+= (float s)			{			x += s; y += s; z += s;							return *this; }
	/// Vector-vector addition.
	inline vec3& operator+= (const vec3& v)		{			x += v.x; y += v.y; z += v.z;					return *this; }
	/// Unary '-' operator. Inverts direction of vector.
	inline vec3 operator- () const				{ vec3 r;	r.x = -x; r.y = -y; r.z = -z;					return r; }
	/// Vector-scalar substraction.
	inline vec3 operator- (float s) const		{ vec3 r;	r.x = x - s; r.y = y - s; r.z = z - s;			return r; }
	/// Vector-vector substraction.
	inline vec3 operator- (const vec3& v) const	{ vec3 r;	r.x = x - v.x; r.y = y - v.y; r.z = z - v.z;	return r; }
	/// Vector-scalar substraction.
	inline vec3& operator-= (float s)			{			x -= s; y -= s; z -= s;							return *this; }
	/// Vector-vector substraction.
	inline vec3& operator-= (const vec3& v)		{			x -= v.x; y -= v.y; z -= v.z;					return *this; }
	/// Vector-scalar multiplication.
	inline vec3 operator* (float k) const		{ vec3 r;	r.x = x*k; r.y = y*k; r.z = z*k;				return r; }
	/// Vector-vector multiplication.
	inline vec3 operator* (const vec3& v) const	{ vec3 r;	r.x = x*v.x; r.y = y*v.y; r.z = z*v.z;			return r; }
	/// Vector-scalar multiplication.
	inline vec3& operator*= (float s)			{			x *= s; y *= s; z *= s;							return *this; }
	/// Vector-vector multiplication.
	inline vec3& operator*= (const vec3& v)		{			x *= v.x; y *= v.y; z *= v.z;					return *this; }
	/// Vector-scalar division.
	inline vec3 operator/ (float k) const		{ vec3 r;	r.x = x*(1.0f/k); r.y = y*(1.0f/k); r.z = z*(1.0f/k);		return r; }
	/// Vector-vector division.
	inline vec3 operator/ (const vec3& v) const	{ vec3 r;	r.x = x*(1.0f/v.x); r.y = y*(1.0f/v.y); r.z = z*(1.0f/v.z);	return r; }
	/// Vector-scalar division.
	inline vec3& operator/= (float s)			{			x *= (1.0f/s); y *= (1.0f/s); z *= (1.0f/s);				return *this; }
	/// Vector-vector division.
	inline vec3& operator/= (const vec3& v)		{			x *= (1.0f/v.x); y *= (1.0f/v.y); z *= (1.0f/v.z);			return *this; }
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
 * @brief Computes the minimum of two vectors.
 *
 * The first component of the result is the minimum value of the
 * first components of the inputs, the second component of the result
 * is the minimum value of the second components of the inputs, ...
 * @param[in] a Input vector.
 * @param[in] b Input vector.
 * @returns Returns the minimum of @e a and @e b.
 */
inline vec3 min(const vec3& a, const vec3& b) {
	vec3 m;
	min(a, b, m);
	return m;
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

/**
 * @brief Computes the maximum of two vectors.
 *
 * The first component of the result is the maximum value of the
 * first components of the inputs, the second component of the result
 * is the maximum value of the second components of the inputs, ...
 * @param[in] a Input vector.
 * @param[in] b Input vector.
 * @returns Returns the maximum of @e a and @e b.
 */
inline vec3 max(const vec3& a, const vec3& b) {
	vec3 M;
	max(a, b, M);
	return M;
}

/* GEOMETRY */

/// The dot product between two vectors.
inline float dot(const vec3& f, const vec3& g)	{ return f.x*g.x + f.y*g.y + f.z*g.z; }

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
	normalise(f, out);
	return out;
}

} // -- namespace math
} // -- namespace physim
