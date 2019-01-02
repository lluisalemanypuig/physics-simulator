#include "kernels.hpp"

using namespace physim;
using namespace math;
using namespace fluids;

namespace kernel_functions {

void density_poly6(float H, kernel_scalar_function& f) {
	f = [H](float r2) -> float
	{
		float k = 1.0f - r2/(H*H);
		float PI = static_cast<float>(M_PI);
		return (315.0f/(64.0f*PI*std::pow(H, 3.0f)))*k*k*k;
	};
}
void pressure_poly6(float H, kernel_vectorial_function& f) {
	f = [H](const vec3& r, float r2, vec3& res) -> void
	{
		static const float PI = static_cast<float>(M_PI);

		float k = 1.0f - r2/(H*H);
		float s = (-945.0f/(32.0f*PI*std::pow(H, 3.0f)))*k*k;
		res = r*s;
	};
}
void viscosity_poly6(float H, kernel_scalar_function& f) {
	f = [H](float r2) -> float
	{
		float k = 1.0f - r2/(H*H);
		float PI = static_cast<float>(M_PI);
		return (945.0f/(8.0f*PI*std::pow(H, 7.0f)))*k*(r2/(H*H) - 0.75f*k);
	};
}

} // -- namespace kernel_functions
