#pragma once

// C++ includes
#include <cstdint>

// physim includes
#include <physim/particles/mesh_particle.hpp>

namespace physim {
namespace meshes {

/**
 * @brief The different types of meshes.
 *
 * The different types of meshes implemented
 * are:
 * - @ref d1
 */
enum class mesh_type : int8_t {
	/**
	 * @brief One-dimensional meshes.
	 *
	 * Or, simply, springs. A particle's movement affects,
	 * most, two particles (its neighbouring particles).
	 */
	d1 = 0
};

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
class mesh {
	protected:
		/// The type of this mesh.
		mesh_type mt;

		/// Total number of particles of the mesh.
		size_t N;
		/// The particles of this mesh.
		particles::mesh_particle **ps;

		/// Elasticity coefficient of each spring.
		float Ke;
		/// Damping factor of each spring.
		float Kd;

	public:
		/// Bouncing coefficient of all the particles in the mesh.
		float bouncing;
		/// Friction coefficient of all the particles in the mesh.
		float friction;

	public:
		/// Default constructor.
		mesh();
		/**
		 * @brief Constructor with parameters.
		 * @param ke Elasticity parameter.
		 * @param kd Damping factor.
		 */
		mesh(float ke, float kd);
		/**
		 * @brief Destructor.
		 *
		 * This method automatically frees the allocated memory
		 * by calling @ref clear.
		 */
		virtual ~mesh();

		// OPERATORS

		/// Returns a reference to the @e i-th particle.
		particles::mesh_particle *operator[] (size_t i);
		/// Returns a constant reference to the @e i-th particle.
		const particles::mesh_particle *operator[] (size_t i) const;

		// MODIFIERS

		/// Frees the memory occupied by this mesh.
		virtual void clear();

		/**
		 * @brief Builds the initial state of the mesh.
		 *
		 * After initialising the mesh's particles, some
		 * initial state may be needed for proper simulation.
		 *
		 * For example, the initial positions of the particles
		 * may have to be stored.
		 *
		 * Each type of mesh has to implement this method
		 * for a proper simulation of that type of mesh.
		 * @pre In general, all this mesh's particles must have
		 * been initialised.
		 */
		virtual void make_initial_state() = 0;

		/**
		 * @brief Update the forces generated within the mesh.
		 *
		 * This method updates the forces acting on all particles.
		 * This update depends on the neighbouring information
		 * determined by the type of mesh.
		 *
		 * This method does not update positions or velocities.
		 *
		 * @pre The modification of the particles' force should
		 * not assume that particles start with null force (force
		 * equal to 0 in the three axes). However, it has to assume
		 * that this method is called before computing the force
		 * due to the force fields.
		 */
		virtual void update_forces() = 0;

		// SETTERS

		/// Sets the elasticity coefficient of this mesh.
		void set_elasticity(float ke);

		/// Sets the damping factor of this mesh.
		void set_damping(float kd);

		// GETTERS

		/// Returns the elasticity coefficient of this mesh.
		float get_elasticity() const;

		/// Returns the damping factor of this mesh.
		float get_damping() const;

		/// Returns the number of particles of this mesh.
		size_t size() const;

		/// Returns the type of this mesh. See @ref mt.
		const mesh_type& get_type() const;

		/// Returns a reference to this mesh's particles.
		particles::mesh_particle **get_particles();

		/// Returns a constant reference to this mesh's particles.
		particles::mesh_particle *const *get_particles() const;
};

} // -- namespace meshes
} // -- namespace physim
