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

#include <physim/fields/gravitational_planet.hpp>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
using namespace particles;
using namespace math;

namespace fields {

// PRIVATE

template<class P>
void gravitational_planet::__compute_force(const P& p, vec3& F) {
	__pm3_mul_v_s(F, pos, p.mass);
}

// PUBLIC

gravitational_planet::gravitational_planet() : punctual() {
}

gravitational_planet::gravitational_planet(const vec3& pos)
	: punctual(pos)
{
}

gravitational_planet::gravitational_planet(const gravitational_planet& f)
	: punctual(f)
{
}

gravitational_planet::~gravitational_planet() {
}

// OTHERS

void gravitational_planet::compute_force(const free_particle& p, vec3& F) {
	__compute_force(p, F);
}

void gravitational_planet::compute_force(const mesh_particle& p, vec3& F) {
	__compute_force(p, F);
}

void gravitational_planet::compute_force(const fluid_particle& p, vec3& F) {
	__compute_force(p, F);
}

} // -- namespace fields
} // -- namespace physim
