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

#include <physim/emitter/free_emitters/rect_fountain.hpp>

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/math/private/math3.hpp>

namespace physim {
using namespace particles;
using namespace math;

namespace emitters {
namespace free_emitters {

// PROTECTED

void rect_fountain::make_vel_init() {
	vel = [this](base_particle& p) {
		const float d2 = __pm3_dist2(p.cur_pos,this->C);
		const float D2 = (this->h*this->h + this->w*this->w)/4.0f;

		vec3 temp;
		__pm3_add_v_v(temp, p.cur_pos, this->n);
		__pm3_sub_v_v(temp, temp, this->C);
		__pm3_normalise(temp, temp);
		__pm3_mul_v_s(p.cur_vel, temp, (D2/d2));
	};
}

// PUBLIC

rect_fountain::rect_fountain() : rect_source() { }

rect_fountain::rect_fountain(const rect_fountain& f) : rect_source(f) {

	// For the same reasons explained in
	// rectangular_source::rectangular_source(const rectangular_source&)
	// make the function that initialises the function again.
	make_vel_init();
}

rect_fountain::~rect_fountain() { }

} // -- namespace free_emitters
} // -- namespace emitters
} // -- namespace physim
