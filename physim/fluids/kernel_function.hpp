#pragma once

// C++ includes
#include <functional>
#include <utility>

// physim includes
#include <physim/particles/fluid_particle.hpp>

namespace physim {
namespace fluids {

/// Shorthand for a kernel function.
typedef
std::function<
	float
	(const particles::fluid_particle *, const particles::fluid_particle *)
> kernel_function;

} // -- namespace fluids
} // -- namespace physim
