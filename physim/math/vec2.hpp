#pragma once

// C++ includes
#include <cmath>

namespace physim {
namespace math {

/**
 * @brief 2-dimensional vector.
 *
 * Basic implementation of a two-dimensional vector as a struct
 * with only two attributes @ref vec2::x, @ref vec2::y.
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
	vec2(float _x,float _y)						{ x = _x; y = _y; }
	/// Copy constructor.
	vec2(const vec2& p)							{ x = p.x; y = p.y; }
	/// Assignation operator.
	vec2& operator= (const vec2& p)				{ x = p.x; y = p.y;							return *this; }
	/// Vector-scalar addition.
	inline vec2 operator+ (float s) const		{ vec2 r;	r.x = x + s; r.y = y + s;		return r; }
	/// Vector-vector addition.
	inline vec2 operator+ (const vec2& p) const	{ vec2 r;	r.x = x + p.x; r.y = y + p.y;	return r; }
	/// Vector-scalar addition.
	inline vec2& operator+= (float s)			{			x += s; y += s;					return *this; }
	/// Vector-vector addition.
	inline vec2& operator+= (const vec2& p)		{			x += p.x; y += p.y;				return *this; }
	/// Unary '-' operator. Inverts direction of vector.
	inline vec2 operator- () const				{ vec2 r;	r.x = -x; r.y = -y;				return r; }
	/// Vector-scalar substraction.
	inline vec2 operator- (float s) const		{ vec2 r;	r.x = x - s; r.y = y - s;		return r; }
	/// Vector-vector substraction.
	inline vec2 operator- (const vec2& p) const	{ vec2 r;	r.x = x - p.x; r.y = y - p.y;	return r; }
	/// Vector-scalar substraction.
	inline vec2& operator-= (float s)			{			x -= s; y -= s;					return *this; }
	/// Vector-vector substraction.
	inline vec2& operator-= (const vec2& p)		{			x -= p.x; y -= p.y;				return *this; }
	/// Vector-scalar multiplication.
	inline vec2 operator* (float k) const		{ vec2 r;	r.x = x*k; r.y = y*k;			return r; }
	/// Vector-vector multiplication.
	inline vec2 operator* (const vec2& p) const	{ vec2 r;	r.x = x*p.x; r.y = y*p.y;		return r; }
	/// Vector-scalar multiplication.
	inline vec2& operator*= (float s)			{			x *= s; y *= s;					return *this; }
	/// Vector-vector multiplication.
	inline vec2& operator*= (const vec2& p)		{			x *= p.x; y *= p.y;				return *this; }
	/// Vector-scalar division.
	inline vec2 operator/ (float k) const		{ vec2 r;	r.x = x*(1.0f/k); r.y = y*(1.0f/k);		return r; }
	/// Vector-vector division.
	inline vec2 operator/ (const vec2& p) const	{ vec2 r;	r.x = x*(1.0f/p.x); r.y = y*(1.0f/p.y);	return r; }
	/// Vector-scalar division.
	inline vec2& operator/= (float s)			{			x *= (1.0f/s); y *= (1.0f/s);			return *this; }
	/// Vector-vector division.
	inline vec2& operator/= (const vec2& p)		{			x *= (1.0f/p.x); y *= (1.0f/p.y);		return *this; }
} vec2;

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
static inline void min(const vec2& a, const vec2& b, vec2& m) {
	m.x = (a.x < b.x ? a.x : b.x);
	m.y = (a.y < b.y ? a.y : b.y);
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
static inline vec2 min(const vec2& a, const vec2& b) {
	vec2 m;
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
static inline void max(const vec2& a, const vec2& b, vec2& M) {
	M.x = (a.x > b.x ? a.x : b.x);
	M.y = (a.y > b.y ? a.y : b.y);
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
static inline vec2 max(const vec2& a, const vec2& b) {
	vec2 M;
	max(a, b, M);
	return M;
}

/* GEOMETRY */

/// The dot product between two vectors.
static inline float dot(const vec2& f, const vec2& g)	{ return f.x*g.x + f.y*g.y; }

/// The square of the norm of a vector.
static inline float norm2(const vec2& f) { return dot(f,f); }
/// The norm of a vector.
static inline float norm(const vec2& f) { return std::sqrt(dot(f,f)); }

/// The squared distance between two points, given their positional vectors.
static inline float dist2(const vec2& f, const vec2& g) {
	return (f.x - g.x)*(f.x - g.x) +
		   (f.y - g.y)*(f.y - g.y);
}
/// The distance between two points, given their positional vectors.
static inline float dist(const vec2& f, const vec2& g) {
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
static inline void perpendicular(const vec2& f, vec2& g) {
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
static inline vec2 perpendicular(const vec2& f) {
	vec2 g; perpendicular(f,g); return g;
}

/**
 * @brief Vector normalisation.
 *
 * Each component of vector @e f is divided by its norm.
 * The result is stored in @e g.
 * @param[in] f Vector to be normalised.
 * @param[out] g Where to store the normalised vector.
 */
static inline void normalise(const vec2& f, vec2& g) {
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
static inline vec2 normalise(const vec2& f) {
	vec2 out;
	normalise(f, out);
	return out;
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
static inline void truncate(const vec2& f, float l, vec2& g) {
	g = f;
	float n2 = norm2(g);
	if (n2 > l*l) {
		g = normalise(f)*l;
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
static inline vec2 truncate(const vec2& f, float l) {
	vec2 g = f;
	float n2 = norm2(g);
	if (n2 > l*l) {
		g = normalise(f)*l;
	}
	return g;
}

} // -- namespace math
} // -- namespace physim
