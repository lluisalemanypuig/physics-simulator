#pragma once

// physim includes
#include <physim/fluids/kernel_function.hpp>

namespace kernel_functions {

void density_poly6(float H, physim::fluids::kernel_scalar_function& f);
void pressure_poly6(float H, physim::fluids::kernel_vectorial_function& f);
void viscosity_poly6(float H, physim::fluids::kernel_scalar_function& f);

} // -- namespace kernel_functions
