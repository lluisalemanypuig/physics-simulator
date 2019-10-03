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
#include <functional>
#include <utility>

// physim includes
#include <physim/particles/fluid_particle.hpp>

namespace physim {
namespace fluids {

/**
 * @brief Shorthand for a scalar kernel function.
 *
 * A kernel function for fluids may be denoted as \f$W(r_{ij})\f$,
 * where \f$r_{ij}\f$ is the distance between two particles.
 *
 * This function uses a maximum distance to define a neighbourhood
 * for a given particle \f$i\f$. Whenever this function is evaluated
 * for a pair of particles \f$i, j\f$ such that the distance between
 * them is larger than this maximum distance, it should return 0.
 *
 * The only parameter is the squared distance between the particles.
 */
typedef
std::function< float (float) > kernel_scalar_function;

/**
 * @brief Shorthand for a vectorial kernel function.
 *
 * A kernel function for fluids may be denoted as \f$W(r_{ij})\f$,
 * where \f$r_{ij}\f$ is the distance between two particles.
 *
 * This function uses a maximum distance to define a neighbourhood
 * for a given particle \f$i\f$. Whenever this function is evaluated
 * for a pair of particles \f$i, j\f$ such that the distance between
 * them is larger than this maximum distance, it should return 0.
 *
 * - First parameter is the vector from particle @e i to particle @e j.
 * - Second parameter is the squared distance between them.
 * - Third parameter is the result of the function.
 */
typedef
std::function<
	void (const physim::math::vec3&, float, physim::math::vec3&)
> kernel_vectorial_function;


} // -- namespace fluids
} // -- namespace physim
