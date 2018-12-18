#pragma once

// C++ includes
#include <cmath>

// physim includes
#include <physim/math/vec2.hpp>

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
static inline void min(const vec3& a, const vec3& b, vec3& m) {
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
static inline vec3 min(const vec3& a, const vec3& b) {
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
static inline void max(const vec3& a, const vec3& b, vec3& M) {
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
static inline vec3 max(const vec3& a, const vec3& b) {
	vec3 M;
	max(a, b, M);
	return M;
}

/* GEOMETRY */

/// The dot product between two vectors.
static inline float dot(const vec3& f, const vec3& g)	{ return f.x*g.x + f.y*g.y + f.z*g.z; }

/// The square of the norm of a vector.
static inline float norm2(const vec3& f) { return dot(f,f); }
/// The norm of a vector.
static inline float norm(const vec3& f) { return std::sqrt(dot(f,f)); }

/// The squared distance between two points, given their positional vectors.
static inline float dist2(const vec3& f, const vec3& g) {
	return (f.x - g.x)*(f.x - g.x) +
		   (f.y - g.y)*(f.y - g.y) +
		   (f.z - g.z)*(f.z - g.z);
}
/// The distance between two points, given their positional vectors.
static inline float dist(const vec3& f, const vec3& g) {
	return std::sqrt(dist2(f,g));
}

/**
 * @brief The cross product of two vectors.
 * @param[out] h The cross product of @e f and @e g.
 * @param[in] f Input vector.
 * @param[in] g Input vector.
 */
static inline void cross(const vec3& f, const vec3& g, vec3& h) {
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
static inline vec3 cross(const vec3& f, const vec3& g) { vec3 h; cross(f,g,h); return h; }

/**
 * @brief Makes a perpendicular vector to @e f.
 *
 * Two vectors are perpendicular if, and only if, their
 * dot product equals 0.
 * @param[out] g A vector perpendicular to @e f.
 * @param[in] f Input vector.
 */
static inline void perpendicular(const vec3& f, vec3& g) {
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
static inline vec3 perpendicular(const vec3& f) {
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
static inline void normalise(const vec3& f, vec3& g) {
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
static inline vec3 normalise(const vec3& f) {
	vec3 out;
	normalise(f, out);
	return out;
}

/**
 * @brief Returns the angle between two vectors.
 * @param[in] f 3D vector.
 * @param[in] g 3D vector.
 * @return Returns the angle between the vectors \f$f,g\f$.
 */
static inline float angle_xyz(const vec3& f, const vec3& g) {
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
static inline float angle_xy(const vec3& f, const vec3& g) {
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
static inline float angle_xz(const vec3& f, const vec3& g) {
	vec3 fxy(f.x,f.y,0.0f);
	return angle_xyz(fxy,g);
}

/**
 * @brief Truncates a vector to a given length.
 *
 * If the length of @e f is larger than l then \f$g = l*\frac{f}{||f||}\f$.
 * If not then \f$g = f\f$.
 * @param[in] f Input vector
 * @param[in] l Maximum length.
 * @param[out] g Truncated vector.
 */
static inline void truncate(const vec3& f, float l, vec3& g) {
	g = f;
	float n2 = norm2(g);
	if (n2 > l*l) {
		g = normalise(g)*l;
	}
}
/**
 * @brief Truncates a vector to a given length.
 *
 * If the length of @e f is larger than l then returns \f$l*\frac{f}{||f||}\f$.
 * If not then returns \f$f\f$.
 * @param f Input vector
 * @param l Maximum length.
 * @return Returns vector @e f truncated to length @e l.
 */
static inline vec3 truncate(const vec3& f, float l) {
	vec3 g = f;
	float n2 = norm2(g);
	if (n2 > l*l) {
		g = normalise(f)*l;
	}
	return g;
}

} // -- namespace math
} // -- namespace physim
