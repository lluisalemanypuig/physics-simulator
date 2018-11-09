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

		// SETTERS

		/**
		 * @brief Sets the number of rows and columns.
		 * @param r Number of rows, at least 2.
		 * @param c Number of columns, at least 2.
		 * @pre The particles of this mesh must have been allocated.
		 * The dimensions used must be equal to @ref N: r*c = N.
		 */
		void set_dimensions(size_t r, size_t c);

		// GETTERS

		/**
		 * @brief Gets the number of rows and columns.
		 * @param[out] r Number of rows.
		 * @param[out] c Number of columns.
		 */
		void get_dimensions(size_t& r, size_t& c) const;

		size_t get_global_index(size_t i, size_t j) const;
};

} // -- namespace meshes
} // -- namespace physim
