#pragma once

// physim includes
#include <physim/particles/mesh_particle.hpp>

namespace physim {
namespace meshes {

/**
 * @brief 1-Dimensional meshes (or springs).
 */
class mesh1d {
	private:
		/// The particles of this mesh.
		particles::mesh_particle **ps;
		/**
		 * @brief Number of particles of the spring.
		 *
		 * Size of the array @ref ps->
		 */
		size_t N;
		/// Elasticity coefficient of each spring.
		float Ke;
		/// Damping factor of each spring.
		float Kd;

	public:
		/// Default constructor.
		mesh1d();
		/// Destructor.
		~mesh1d();

		// OPERATORS

		/**
		 * @brief Operator =.
		 *
		 * This new mesh is initialised with as many particles
		 * as @e m has. Then, each particle's attributes are
		 * copied into the new ones.
		 * @param m Mesh to be copied.
		 */
		mesh1d& operator= (const mesh1d& m);

		// MODIFIERS

		/**
		 * @brief Initialises the spring with @e n particles.
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
		 * index 1, ...
		 */
		void init(size_t n);

		/// Frees the memory occupied by this mesh.
		void clear();

		// SETTERS

		/// Sets elasticity coefficient of the mesh. See @ref Ke.
		void set_elasticity(float ke);

		/// Sets damping factor of the mesh. See @ref Kd.
		void set_damping(float kd);

		// GETTERS

		/// Returns the elasticity coefficient of the mesh. See @ref Ke.
		float get_elasticity() const;

		/// Returns the damping factor of the mesh. See @ref Kd.
		float get_damping() const;

		/// Returns the number of particles of this mesh.
		size_t n_particles() const;

		/// Returns a reference to this spring's particles.
		particles::mesh_particle **get_particles();
};

} // -- namespace meshes
} // -- namespace physim
