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

#include <physim/fields/punctual.hpp>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace fields {

punctual::punctual() {
	__pm3_assign_s(pos, 0.0f);
}

punctual::punctual(const math::vec3& p) : field() {
	__pm3_assign_v(pos, p);
}

punctual::punctual(const punctual& f) : field(f) {
	__pm3_assign_v(pos, f.pos);
}

punctual::~punctual() {

}

// SETTERS

void punctual::set_position(const math::vec3& p) {
	__pm3_assign_v(pos, p);
}

// GETTERS

const math::vec3& punctual::get_position() const {
	return pos;
}

} // -- namespace fields
} // -- namespace physim
