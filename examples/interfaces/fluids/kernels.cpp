/*********************************************************************
 * Real-time physics simulation project
 * Copyright (C) 2018-2019 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

#include "kernels.hpp"

using namespace physim;
using namespace math;
using namespace fluids;

static
const float PI = static_cast<float>(M_PI);

#define inv(x) (1.0f/(x))

namespace kernel_functions {

void density_poly6(float H, kernel_scalar_function& f) {
	f = [H](float r2) -> float
	{
		float k = inv(H) - r2*inv(H*H*H);
		return 315.0f*inv(64.0f*PI)*k*k*k;
	};
}
void density_spline(float H, kernel_scalar_function& f) {
	f = [H](float r2) -> float
	{
		float r = std::sqrt(r2);
		float C;

		if (r*inv(H) <= 1.0f) {
			C = 1.0f - 1.5f*r2*inv(H*H) + 0.75f*r*r*r*inv(H*H*H);
		}
		else {
			C = 0.25f*std::pow(2.0f - r*inv(H), 3.0f);
		}

		return inv(PI*H*H*H)*C;
	};
}

void pressure_poly6(float H, kernel_vectorial_function& f) {
	f = [H](const vec3& r, float r2, vec3& res) -> void
	{
		float k = 1.0f - r2*inv(H*H);
		float s = -945.0f*inv(32.0f*PI*std::pow(H, 5.0f))*k*k;
		res = r*s;
	};
}
void pressure_spiky(float H, kernel_vectorial_function& f) {
	f = [H](const vec3& r, float r2, vec3& res) -> void
	{
		vec3 rnorm = normalise(r);
		float s = -45.0f*inv(PI*std::pow(H, 5.0f))*(1.0f - r2*inv(H));
		res = rnorm*s;
	};
}

void viscosity_poly6(float H, kernel_scalar_function& f) {
	f = [H](float r2) -> float
	{
		float r2_h2 = r2*inv(H*H);
		float C = r2_h2*(10.0f - 7.0f*r2_h2) - 3.0f;
		return 945.0f*inv(32.0f*PI*std::pow(H, 5.0f))*C;
	};
}
void viscosity_spiky(float H, kernel_scalar_function& f) {
	f = [H](float r2) -> float
	{
		float s = H - std::sqrt(r2);
		return 45.0f*inv(PI*std::pow(H, 6.0f))*s;
	};
}

} // -- namespace kernel_functions
