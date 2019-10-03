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

#include <physim/fields/magnetic_B.hpp>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
using namespace particles;
using namespace math;

namespace fields {

// PRIVATE

template<class P>
void magnetic_B::__compute_force(const P& p, vec3& F) {
	vec3 temp;
	__pm3_mul_v_s(temp, p.cur_vel, p.charge);
	__pm3_cross(F, temp, B);
}

// PUBLIC

magnetic_B::magnetic_B() : magnetic() { }

magnetic_B::magnetic_B(const vec3& pos, const vec3& b)
	: magnetic(pos, b)
{

}

magnetic_B::magnetic_B(const magnetic_B& f)
	: magnetic(f)
{

}

magnetic_B::~magnetic_B() {

}

// OTHERS

void magnetic_B::compute_force(const free_particle& p, vec3& F) {
	__compute_force(p, F);
}

void magnetic_B::compute_force(const mesh_particle& p, vec3& F) {
	__compute_force(p, F);
}

void magnetic_B::compute_force(const fluid_particle&, vec3&) {
	// do nothing
}

} // -- namespace fields
} // -- namespace physim
