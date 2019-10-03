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

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/particles/mesh_particle.hpp>
#include <physim/particles/fluid_particle.hpp>

namespace physim {
namespace particles {

	/**
	 * @brief Conversion between types of particles.
	 *
	 * Only the common attributes are copied.
	 * @param[in] m From a mesh particle.
	 * @param[out] f To a free particle.
	 */
	void from_mesh_to_free(const mesh_particle& m, free_particle& f);
	/**
	 * @brief Conversion between types of particles.
	 *
	 * Only the common attributes are copied.
	 * @param[in] f From a free particle.
	 * @param[out] m To a mesh particle.
	 */
	void from_free_to_mesh(const free_particle& f, mesh_particle& m);

	/**
	 * @brief Conversion between types of particles.
	 *
	 * Only the common attributes are copied.
	 * @param[in] fl From a fluid particle.
	 * @param[out] fr To a free particle.
	 */
	void from_fluid_to_free(const fluid_particle& fl, free_particle& fr);
	/**
	 * @brief Conversion between types of particles.
	 *
	 * Only the common attributes are copied.
	 * @param[in] fr From a free particle.
	 * @param[out] fl To a fluid particle.
	 */
	void from_free_to_fluid(const free_particle& fr, fluid_particle& fl);

} // -- namespace particles
} // -- namespace physim
