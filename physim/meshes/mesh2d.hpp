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
#include <vector>

// physim includes
#include <physim/particles/mesh_particle.hpp>
#include <physim/meshes/mesh.hpp>

namespace physim {
namespace meshes {

/**
 * @brief 2-Dimensional spring mesh.
 *
 * A 2-dimensional mesh does not differ much from a 3-dimensional
 * mesh. However, these are used mainly for cloth simulation.
 */
class mesh2d : public mesh {
	private:

	public:
		/// Default constructor.
		mesh2d();
		/**
		 * @brief Constructor with parameters.
		 * @param ke Elasticity parameter.
		 * @param kd Damping factor.
		 */
		mesh2d(float ke, float kd);
		/// Destructor.
		virtual ~mesh2d();

};

} // -- namespace meshes
} // -- namespace physim
