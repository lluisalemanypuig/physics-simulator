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
 
#pragma once

// C++ includes
#include <cstdint>

namespace physim {
namespace particles {

/// The different types of particles.
enum class particle_type : int8_t {
	/// Base particle, see @ref base_particle.
	base_particle = 0,
	/// Free particle, see @ref free_particle.
	free_particle,
	/// Sized particle, see @ref sized_particle.
	sized_particle,
	/// Mesh particle, see @ref mesh_particle.
	mesh_particle,
	/// Fluid particle, see @ref fluid_particle.
	fluid_particle,
	/// Agent particle, see @ref agent_particle.
	agent_particle
};

} // -- namespace particles
} // -- namespace physim
