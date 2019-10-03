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

#include <physim/particles/fluid_particle.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace particles {

// PRIVATE

void fluid_particle::partial_init() {
	density = 0.0f;
	pressure = 0.0f;
}

// PUBLIC

fluid_particle::fluid_particle() : base_particle() {
	partial_init();
}

fluid_particle::fluid_particle(const fluid_particle& p) : base_particle(p) {
	density = p.density;
	pressure = p.pressure;
}

fluid_particle::~fluid_particle() { }

// MODIFIERS

void fluid_particle::init() {
	base_particle::init();
	partial_init();
}

particle_type fluid_particle::get_particle_type() const {
	return particle_type::fluid_particle;
}

} // -- namespace particles
} // -- namespace physim
