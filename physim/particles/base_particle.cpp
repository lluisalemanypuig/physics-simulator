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

#include <physim/particles/base_particle.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace particles {

base_particle::base_particle() {
	init();
}

base_particle::base_particle(const base_particle& p) {
	__pm3_assign_v(prev_pos, p.prev_pos);
	__pm3_assign_v(cur_pos, p.cur_pos);
	__pm3_assign_v(cur_vel, p.cur_vel);
	__pm3_assign_v(force, p.force);
	mass = p.mass;
	index = p.index;
}

base_particle::~base_particle() {

}

// MODIFIERS

void base_particle::save_position() {
	__pm3_assign_v(prev_pos, cur_pos);
}

void base_particle::init() {
	__pm3_assign_s(prev_pos, 0.0f);
	__pm3_assign_s(cur_pos, 0.0f);
	__pm3_assign_s(cur_vel, 0.0f);
	__pm3_assign_s(force, 0.0f);
	mass = 1.0f;
}

particle_type base_particle::get_particle_type() const {
	return particle_type::base_particle;
}

} // -- namespace particles
} // -- namespace physim
