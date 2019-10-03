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

#include <physim/fields/magnetic.hpp>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace fields {

magnetic::magnetic() : punctual() {
	__pm3_assign_s(B, 0.0f);
}

magnetic::magnetic(const math::vec3& p, const math::vec3& b)
	: punctual(p)
{
	__pm3_assign_v(B, b);
}

magnetic::magnetic(const magnetic& f)
	: punctual(f)
{
	__pm3_assign_v(B, f.B);
}

magnetic::~magnetic() {
}

// SETTERS

void magnetic::set_vector(const math::vec3& v) {
	__pm3_assign_v(B, v);
}

// GETTERS

const math::vec3& magnetic::get_vector() const {
	return B;
}

} // -- namespace fields
} // -- namespace physim






