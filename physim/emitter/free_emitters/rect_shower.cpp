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

#include <physim/emitter/free_emitters/rect_shower.hpp>

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/math/private/math3.hpp>

namespace physim {
using namespace particles;

namespace emitters {
namespace free_emitters {

// PROTECTED

void rect_shower::make_vel_init() {
	vel = [](base_particle& p) {
		__pm3_assign_s(p.cur_vel, 0.0f);
	};
}

// PUBLIC

rect_shower::rect_shower() : rect_source() { }

rect_shower::rect_shower(const rect_shower& w) : rect_source(w) {

}

rect_shower::~rect_shower() { }

// GETTERS

free_emitter *rect_shower::clone() const {
	return new rect_shower(*this);
}

} // -- namespace free_emitters
} // -- namespace emitters
} // -- namespace physim
