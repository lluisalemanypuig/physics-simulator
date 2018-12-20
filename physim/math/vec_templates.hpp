#pragma once

namespace physim {
namespace math {

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
template<class T>
static inline T min(const T& a, const T& b) {
	T m;
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
 * @returns Returns the maximum of @e a and @e b.
 */
template<class T>
static inline T max(const T& a, const T& b) {
	T M;
	max(a, b, M);
	return M;
}

/// The distance between two points, given their positional vectors.
template<class T>
static inline float dist(const T& f, const T& g) {
	return std::sqrt(dist2(f,g));
}

/// The norm of a vector.
template<class T>
static inline float norm(const T& f) { return std::sqrt(dot(f,f)); }

/// The square of the norm of a vector.
template<class T>
static inline float norm2(const T& f) { return dot(f,f); }

/**
 * @brief Vector normalisation.
 *
 * Each component of vector @e f is divided by its norm.
 * @param f Vector to be normalised.
 * @returns Returns the normalisation of vector @e f.
 */
template<class T>
static inline T normalise(const T& f) {
	T out;
	normalise(f, out);
	return out;
}

/**
 * @brief Truncates a vector to a maximum length.
 *
 * If the length of @e f is larger than l then \f$g = l*\frac{f}{||f||}\f$.
 * If not then \f$g = f\f$.
 * @param[in] f Input vector
 * @param[in] l Maximum length.
 * @param[out] g Truncated vector.
 */
template<class T>
static inline void truncate(const T& f, float l, T& g) {
	g = f;
	float n2 = norm2(g);
	if (n2 > l*l) {
		normalise(g, g);
		g *= l;
	}
}
/**
 * @brief Truncates a vector to a maximum length.
 *
 * If the length of @e f is larger than l then returns \f$l*\frac{f}{||f||}\f$.
 * If not then returns \f$f\f$.
 * @param f Input vector
 * @param l Maximum length.
 * @return Returns vector @e f truncated to maximum length @e l.
 */
template<class T>
static inline T truncate(const T& f, float l) {
	T g = f;
	float n2 = norm2(g);
	if (n2 > l*l) {
		normalise(g, g);
		g *= l;
	}
	return g;
}

/**
 * @brief Truncates a vector to a given length.
 *
 * The length of the returned vector is @e l.
 * @param[in] f Input vector
 * @param[in] l Maximum length.
 * @param[out] g Truncated vector.
 */
template<class T>
static inline void truncate_to(const T& f, float l, T& g) {
	normalise(f, g);
	g *= l;
}
/**
 * @brief Truncates a vector to a given length.
 *
 * The length of the returned vector is @e l.
 * @param f Input vector
 * @param l Maximum length.
 * @return Returns vector @e f truncated to length @e l.
 */
template<class T>
static inline T truncate_to(const T& f, float l) {
	T g;
	normalise(f, g);
	g *= l;
	return g;
}

} // -- namespace math
} // -- namespace physim
