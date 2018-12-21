#pragma once

// C++ includes
#include <functional>
#include <utility>

// physim includes
#include <physim/fluids/kernel_function.hpp>

namespace physim {
namespace fluids {

/**
 * @brief Shorthand for a kernel pair.
 *
 * A kernel pair is a kernel function and its smoothing length.
 */
typedef struct {
	/// Smoothing length of the kernel function.
	float radius;
	/// Kernel function.
	kernel_function F;
} kernel_pair;

} // -- namespace fluids
} // -- namespace physim


