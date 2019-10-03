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

#include <physim/particles/free_particle.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace particles {

// PRIVATE

void free_particle::partial_init() {
	friction = 0.2f;
	bouncing = 0.8f;
	charge = 0.0f;
	lifetime = 10.0f;
	starttime = 0.0f;
	fixed = false;
}

// PUBLIC

free_particle::free_particle() : base_particle() {
	partial_init();
}

free_particle::free_particle(const free_particle& p) : base_particle(p) {
	bouncing = p.bouncing;
	friction = p.friction;
	charge = p.charge;
	lifetime = p.lifetime;
	starttime = p.starttime;
	fixed = p.fixed;
}

free_particle::~free_particle() { }

// MODIFIERS

void free_particle::reduce_lifetime(float t) {
	assert(t >= 0.0f);
	lifetime -= t;
}

void free_particle::reduce_starttime(float t) {
	assert(t >= 0.0f);
	starttime -= t;
}

void free_particle::init() {
	base_particle::init();
	partial_init();
}

particle_type free_particle::get_particle_type() const {
	return particle_type::free_particle;
}

} // -- namespace particles
} // -- namespace physim
