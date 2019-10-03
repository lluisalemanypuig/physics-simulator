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

#include <physim/emitter/sized_emitter.hpp>

// physim includes
#include <physim/particles/particle_types.hpp>
#include <physim/math/private/math3.hpp>

// Shortcut for base_particle.
typedef physim::particles::sized_particle spart;

namespace physim {
using namespace particles;

namespace emitters {

// PRIVATE

// PUBLIC

sized_emitter::sized_emitter() : free_emitter() {
	radius = [](spart& p) { p.R = 1.0f; };
}

sized_emitter::sized_emitter(const sized_emitter& i) : free_emitter(i) {
	radius = i.radius;
}

sized_emitter::~sized_emitter() { }

// SETTERS

void sized_emitter::set_radius_initialiser(const sized_emit& f) {
	radius = f;
}

// GETTERS

sized_emitter *sized_emitter::clone() const {
	return new sized_emitter(*this);
}

const sized_emit& sized_emitter::get_radius_initialiser() const {
	return radius;
}

// INITIALISE A PARTICLE

void sized_emitter::initialise_particle(particles::sized_particle& p) const {
	free_emitter::initialise_particle(p);
	radius(p);
}

} // -- namespace emitters
} // -- namespace physim
