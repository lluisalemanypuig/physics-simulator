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
 * @brief 1-Dimensional spring mesh.
 *
 * This represents a set of @ref particles::mesh_particle
 * where one particle is attached to at most two other
 * particles.
 *
 * If the local index of a particle is 0, then
 * that particle is connected only to particle with index 1.
 * Similarly, the particle with local index N - 1 is only
 * connected to that with index N - 2. The other particles
 * are connected to the particles with index \f$\pm 1\f$.
 */
class mesh1d : public mesh {
	private:
		/**
		 * @brief Original distances between the particles.
		 *
		 * Function @ref make_initial_state fills this vector with the distances
		 * between the particles' positions that they had prior to calling this
		 * method. The particles used for computing these distances are neighbouring
		 * particles.
		 *
		 * The contents are formally defined as:
		 * stretch_ds[i].x = distance(particle i, particle i + 1)
		 * for i ranging in [0, N - 2].
		 *
		 * stretch_ds[i].y = distance(particle i, particle i + 2)
		 * for i ranging in [0, N - 3].
		 *
		 * The distances in the .x members are used for stretch forces.
		 *
		 * The distances in the .y members are used for bend forces.
		 */
		math::vec2 *ds;


	public:
		/// Default constructor.
		mesh1d();
		/**
		 * @brief Constructor with parameters.
		 * @param ke Elasticity parameter.
		 * @param kd Damping factor.
		 */
		mesh1d(float ke, float kd);
		/// Destructor.
		~mesh1d();

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
};

} // -- namespace meshes
} // -- namespace physim
