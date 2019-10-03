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
#include <physim/math/math.hpp>
#include <physim/meshes/mesh.hpp>

namespace physim {
namespace meshes {

/**
 * @brief 2-Dimensional regular spring mesh.
 *
 * This mesh's springs are organised in a grid: the particles at
 * the four corners have two neighbours, the particles at the border
 * of the grid (without considering the corners) have three neighbours,
 * and the rest have four.
 *
 * By neighbours it should be understood the immediate particle in
 * the grid. That is, in a grid of @e R and @e C particles in each
 * dimension the particle at \f$(i,j)\:|\: 0 \le i \le R - 1, 0 \le j \le C - 1\f$
 * has as neighbours the particles at \f$(i-1,j),(i+1,j),(i,j-1),(i,j+1)\f$.
 *
 * In this type of meshes, there three types of internal forces
 * implemented by default:
 * - stretch: @ref stretch.
 * - shear: @ref shear.
 * - bending: @ref bend.
 * These internal forces can be activated through the functions
 * - @ref simulate_stretch.
 * - @ref simulate_shear.
 * - @ref simulate_bend.
 * with an appropriate value of their parameter.
 * Depending on the type of mesh some of these forces may not be
 * implemented.
 */
class mesh2d_regular : public mesh {
	private:
		/// Number of rows of the grid.
		size_t R;
		/// Number of columns of the grid.
		size_t C;

		/**
		 * @brief Original distances between the particles.
		 *
		 * Function @ref make_initial_state fills this vector
		 * with the distances between the particles that they
		 * had prior to calling this method.
		 *
		 * For a particle (@e i, @e j) the original distances are:
		 * - Stretch
		 *	- distance to (i, j + 1): in member @e x.
		 *	- distance to (i + 1, j): in member @e y.
		 * for \f$0 \le i \le R - 2, 0 \le j \le C - 2\f$.
		 *
		 * - Bend
		 *	- distance to (i, j + 2): in member @e z.
		 *	- distance to (i + 2, j): in member @e u.
		 * for \f$0 \le i \le R - 3, 0 \le j \le C - 3\f$.
		 *
		 * - Shear
		 *	- distance to (i - 1, j + 1): in member @e v.
		 * for \f$1 \le i \le R - 1, 0 \le j \le C - 2\f$.
		 *	- distance to (i + 1, j + 1): in member @e w.
		 * for \f$0 \le i \le R - 2, 0 \le j \le C - 2\f$.
		 *
		 * where @e C is the number of columns (see @ref C),
		 * and @e R is the number of rows (see @ref R).
		 *
		 * As a side note, there are some members that are not
		 * used at all. These are:
		 * - members @e z,@e u, for particles \f$(R-2,j), (i,C-2)\f$, for
		 * \f$0 \le i \le R - 2\f$ and \f$0 \le j \le C - 2\f$.
		 * - members @e v, for particles \f$(0,j)\f$, for \f$0 \le j \le C - 2\f$.
		 * - members @e w, for particles \f$(R-1,j)\f$, for \f$0 \le j \le C - 2\f$.
		 *
		 * where @e C is the number of columns (see @ref C),
		 * and @e R is the number of rows (see @ref R).
		 */
		math::vec6 *sb_ds;

		/// Simulate stretch forces.
		bool stretch;
		/// Simulate shear forces.
		bool shear;
		/// Simulate bend forces.
		bool bend;

	public:
		/// Default constructor.
		mesh2d_regular();
		/**
		 * @brief Constructor with parameters.
		 * @param ke Elasticity parameter.
		 * @param kd Damping factor.
		 */
		mesh2d_regular(float ke, float kd);
		/// Destructor.
		virtual ~mesh2d_regular();

		// MODIFIERS

		/**
		 * @brief Builds the initial state of the mesh.
		 *
		 * Records the initial distances between neighbouring
		 * particles.
		 *
		 * @pre All this mesh's particles must have been
		 * initialised.
		 */
		void make_initial_state();

		/**
		 * @brief Update the forces generated within the mesh.
		 *
		 * This method updates the forces acting on a particle @e i
		 * and its neighbouring particles. This includes stretch and
		 * beding forces.
		 *
		 * @pre The modification of the particle's forces should
		 * not assume that particles start with null force (force
		 * equal to 0 in the three axes). Moreover, the initial state
		 * of the mesh must have been made (see @ref make_initial_state).
		 */
		void update_forces();

		void clear();

		/**
		 * @brief Activates/Deactivates the simulation of stretch internal forces.
		 *
		 * Sets @ref stretch to the value of @e s.
		 * @param s True or false depending on whether the simulation
		 * has to activated or deactivated.
		 */
		void simulate_stretch(bool s = true);
		/**
		 * @brief Activates/Deactivates the simulation of shear internal forces.
		 *
		 * Sets @ref shear to the value of @e s.
		 * @param s True or false depending on whether the simulation
		 * has to activated or deactivated.
		 */
		void simulate_shear(bool s = true);
		/**
		 * @brief Activates/Deactivates the simulation of bending internal forces.
		 *
		 * Sets @ref bend to the value of @e s.
		 * @param s True or false depending on whether the simulation
		 * has to activated or deactivated.
		 */
		void simulate_bend(bool s = true);

		// SETTERS

		/**
		 * @brief Sets the number of rows and columns.
		 * @param r Number of rows, at least 2.
		 * @param c Number of columns, at least 2.
		 * @pre The particles of this mesh must have been allocated.
		 * The dimensions used must be equal to @ref N : r*c = N.
		 */
		void set_dimensions(size_t r, size_t c);

		// GETTERS

		/**
		 * @brief Returns whether stretch forces are actived.
		 * @return Returns the value of @ref stretch.
		 */
		bool is_simulating_stretch() const;
		/**
		 * @brief Returns whether shear forces are actived.
		 * @return Returns the value of @ref shear.
		 */
		bool is_simulating_shear() const;
		/**
		 * @brief Returns whether bend forces are actived.
		 * @return Returns the value of @ref bend.
		 */
		bool is_simulating_bend() const;

		/**
		 * @brief Gets the number of rows and columns.
		 * @param[out] r Number of rows.
		 * @param[out] c Number of columns.
		 */
		void get_dimensions(size_t& r, size_t& c) const;

		/**
		 * @brief Returns the index of a particle of the mesh.
		 * @param i Row index.
		 * @param j Column index.
		 * @return Returns the index of the particle at row @e i and
		 * column @e j as a single integer.
		 */
		size_t get_global_index(size_t i, size_t j) const;
};

} // -- namespace meshes
} // -- namespace physim
