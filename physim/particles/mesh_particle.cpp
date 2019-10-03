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

#include <physim/particles/mesh_particle.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace particles {

// PRIVATE

void mesh_particle::partial_init() {
	charge = 0.0f;
	fixed = false;
}

// PUBLIC

mesh_particle::mesh_particle() : base_particle() {
	partial_init();
}

mesh_particle::mesh_particle(const mesh_particle& p) : base_particle(p) {
	charge = p.charge;
	fixed = p.fixed;
}

mesh_particle::~mesh_particle() { }

// MODIFIERS

void mesh_particle::init() {
	base_particle::init();
	partial_init();
}

particle_type mesh_particle::get_particle_type() const {
	return particle_type::mesh_particle;
}

} // -- namespace particles
} // -- namespace physim
