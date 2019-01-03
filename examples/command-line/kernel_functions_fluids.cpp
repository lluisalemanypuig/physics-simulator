#include "kernel_functions_fluids.hpp"

using namespace physim;
using namespace math;
using namespace fluids;

static
const float PI = static_cast<float>(M_PI);

namespace kernel_functions {

void density_poly6(float H, kernel_scalar_function& f) {
	f = [H](float r2) -> float
	{
		float k = 1.0f/H - r2*(1.0f/(H*H*H));
		return (315.0f/(64.0f*PI))*k*k*k;
	};
}
void pressure_poly6(float H, kernel_vectorial_function& f) {
	f = [H](const vec3& r, float r2, vec3& res) -> void
	{
		float k = 1.0f - r2/(H*H);
		float s = (-945.0f/(32.0f*PI*std::pow(H, 5.0f)))*k*k;
		res = r*s;
	};
}
void viscosity_poly6(float H, kernel_scalar_function& f) {
	f = [H](float r2) -> float
	{
		float k = 1.0f - r2/(H*H);
		return (945.0f/(8.0f*PI*std::pow(H, 5.0f)))*k*(r2/(H*H) - 0.75f*k);
	};
}

} // -- namespace kernel_functions
