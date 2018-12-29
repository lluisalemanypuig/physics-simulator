#pragma once

// C++ includes
#include <functional>
#include <utility>

// physim includes
#include <physim/particles/fluid_particle.hpp>

namespace physim {
namespace fluids {

/**
 * @brief Shorthand for a scalar kernel function.
 *
 * A kernel function for fluids may be denoted as \f$W(r_{ij})\f$,
 * where \f$r_{ij}\f$ is the distance between two particles.
 *
 * This function uses a maximum distance to define a neighbourhood
 * for a given particle \f$i\f$. Whenever this function is evaluated
 * for a pair of particles \f$i, j\f$ such that the distance between
 * them is larger than this maximum distance, it should return 0.
 *
 * The only parameter is the squared distance between the particles.
 */
typedef
std::function< float (float) > kernel_scalar_function;

/**
 * @brief Shorthand for a vectorial kernel function.
 *
 * A kernel function for fluids may be denoted as \f$W(r_{ij})\f$,
 * where \f$r_{ij}\f$ is the distance between two particles.
 *
 * This function uses a maximum distance to define a neighbourhood
 * for a given particle \f$i\f$. Whenever this function is evaluated
 * for a pair of particles \f$i, j\f$ such that the distance between
 * them is larger than this maximum distance, it should return 0.
 *
 * - First parameter is the vector from particle @e i to particle @e j.
 * - Second parameter is the squared distance between them.
 * - Third parameter is the result of the function.
 */
typedef
std::function<
	void (const physim::math::vec3&, float, physim::math::vec3&)
> kernel_vectorial_function;


} // -- namespace fluids
} // -- namespace physim
