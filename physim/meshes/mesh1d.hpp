#pragma once

// C++ includes
#include <vector>

// physim includes
#include <physim/particles/mesh_particle.hpp>
#include <physim/meshes/mesh.hpp>

namespace physim {
namespace meshes {

/**
 * @brief Abstract spring mesh.
 *
 * This represents a set of @ref particles::mesh_particle
 * where one particle is attached to some other particles
 * of the same mesh with a spring. The movement of one
 * particle in the spring, then, affects the other particles
 * it is attached to.
 *
 * All the springs of the mesh are described likewise with
 * the following coefficients:
 * - elasticity parameter (see @ref Ke).
 * - damping factor (see @ref Kd).
 */
class mesh1d : public mesh {
	private:
		std::vector<float> ds;

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
		 * and its neighbouring particles. The neighbouring particles
		 * of a particle @e i are:
		 * - \f$i + 1\f$ if \f$i=0\f$,
		 * - \f$i - 1\f$ if \f$i=N-1\f$, (where \f$N\f$ is @ref mesh::N)
		 * - \f$i - 1\f$ and \f$i + 1\f$ if otherwise.
		 *
		 * @pre The modification of the particle's forces should
		 * not assume that particles start with null force (force
		 * equal to 0 in the three axes).
		 */
		void update_forces();

		/**
		 * @brief Allocates memory for @e n particles.
		 *
		 * The previous contents of this spring are cleared
		 * (see @ref clear).
		 *
		 * Every particle is initialised with its default
		 * constructor. After that, they are assigned an
		 * index within the mesh.
		 *
		 * This index is local, that is, the first particle
		 * of every 1-dimensional mesh has index 0, the second
		 * particle has index 1, ...
		 */
		void allocate(size_t n);

		void clear();
};

} // -- namespace meshes
} // -- namespace physim
