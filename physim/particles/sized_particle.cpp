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

#include <physim/particles/sized_particle.hpp>

namespace physim {
namespace particles {

// PRIVATE

void sized_particle::partial_init() {
	R = 1.0f;
}

// PUBLIC

sized_particle::sized_particle() : free_particle() {
	partial_init();
}

sized_particle::sized_particle(const sized_particle& p) : free_particle(p) {
	R = p.R;
}

sized_particle::~sized_particle() {
}

// MODIFIERS

void sized_particle::init() {
	free_particle::init();
	partial_init();
}

// GETTERS

particle_type sized_particle::get_particle_type() const {
	return particle_type::sized_particle;
}

} // -- namespace particles
} // -- namespace physim
