#pragma once

// C++ includes
#include <functional>
#include <utility>

// physim includes
#include <physim/particles/fluid_particle.hpp>

namespace physim {
namespace fluids {

/**
 * @brief Shorthand for a kernel function.
 *
 * A kernel function for fluids may be denoted as \f$W(r_{ij})\f$,
 * where \f$r_{ij}\f$ is the distance between two particles.
 *
 * This function uses a maximum distance to define a neighbourhood
 * for a given particle \f$i\f$. Whenever this function is evaluated
 * for a pair of particles \f$i, j\f$ such that the distance between
 * them is larger than this maximum distance, it should return 0.
 *
 * - First parameter is guaranteed to be particle @e i.
 * - Second parameter is guaranteed to be particle @e j.
 * - Third parameter is the squared distance between them.
 */
typedef
std::function<
	float
	(const particles::fluid_particle&,
	 const particles::fluid_particle&,
	 float)
> kernel_function;

} // -- namespace fluids
} // -- namespace physim
