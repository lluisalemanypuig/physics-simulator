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
 * - @ref mesh_type::d1
 */
enum class mesh_type : int8_t {
	/**
	 * @brief 1-dimensional meshes.
	 *
	 * Or, simply, springs. A particle's movement affects,
	 * most, two particles (its neighbouring particles).
	 */
	d1 = 0,
	/**
	 * @brief 2-dimensional meshes.
	 *
	 * Each non-border particle has 4 neighbours. Particles
	 * at the corners have 2 neighbours and the rest have 3.
	 */
	d2_regular
};

/**
 * @brief Abstract spring mesh.
 *
 * This represents a set of @ref particles::mesh_particle
 * where one particle is attached to some other particles
 * of the same mesh with a spring. The movement of one
 * particle in the spring, then, affects the other particles
 * it is attached to. Each particle has a local index @e i
 * that ranges between 0 and the number of particles - 1.
 *
 * All the springs of the mesh are described similarly with
 * the following coefficients:
 * - elasticity parameter (see @ref Ke).
 * - damping factor (see @ref Kd).
 *
 * Some coefficients are common to all the particles:
 * - bouncing coefficient (see @ref bouncing).
 * - friction coefficient (see @ref friction).
 *
 * There are internal forces that can be simulated. These are
 * dependent on each type of mesh.
 */
class mesh {
	protected:
		/// The type of this mesh.
		mesh_type mt;

		/// Total number of particles of the mesh.
		size_t N;
		/// The particles of this mesh.
		particles::mesh_particle *ps;

		/// Elasticity coefficient of each spring.
		float Ke;
		/// Damping factor of each spring.
		float Kd;

		/// Bouncing coefficient of all the particles in the mesh.
		float bouncing;
		/// Friction coefficient of all the particles in the mesh.
		float friction;

	public:
		/// Default constructor.
		mesh();
		/**
		 * @brief Constructor with parameters.
		 *
		 * The meshe's attributes are initialised to:
		 * - @ref Ke : 100
		 * - @ref Kd : 0.05
		 * - @ref bouncing : 0.8
		 * - @ref friction : 0.2
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
		particles::mesh_particle& operator[] (size_t i);
		/// Returns a constant reference to the @e i-th particle.
		const particles::mesh_particle& operator[] (size_t i) const;

		// MODIFIERS

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
		 *
		 * The mass of each particle can also be initialised
		 * at this point. The mass of the mesh (param @e Kg)
		 * is divided uniformly among the @e n particles.
		 * @param n Number of particles.
		 * @param Kg Mass of the mesh.
		 */
		void allocate(size_t n, float Kg = 1.0f);

		/// Frees the memory occupied by this mesh.
		virtual void clear();

		/**
		 * @brief Builds the initial state of the mesh.
		 *
		 * After assigning each of this mesh's particles a position,
		 * the initial state needed for proper simulation is computed.
		 *
		 * This may include the initial distances between the particles
		 * at each end of every simulated spring.
		 *
		 * Each type of mesh has to implement this method
		 * for a proper simulation of that type of mesh.
		 * @pre In general, all this mesh's particles must have
		 * been initialised (say, given an initial current position).
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
		 * @pre The modification of a particles' force may assume
		 * that all particles start with null force (force
		 * equal to 0 in the three axes).
		 * @pre This method is called before computing the forces
		 * acting on the particles due to force fields.
		 */
		virtual void update_forces() = 0;

		// SETTERS

		/// Sets the elasticity coefficient of this mesh.
		void set_elasticity(float ke);
		/// Sets the damping factor of this mesh.
		void set_damping(float kd);

		/// Sets the friction coefficient of this mesh.
		void set_friction(float ke);
		/// Sets the bouncing factor of this mesh.
		void set_bouncing(float kd);

		/**
		 * @brief Sets the mass (in Kg) of the mesh.
		 *
		 * The mass is divided uniformly among the particles of the mesh.
		 * This method can only be called after allocating
		 * @param Kg Mass of the mesh.
		 */
		void set_mass(float Kg);

		// GETTERS

		/// Returns the elasticity coefficient of this mesh.
		float get_elasticity() const;
		/// Returns the damping factor of this mesh.
		float get_damping() const;

		/// Returns the friction coefficient of this mesh.
		float get_friction() const;
		/// Returns the bouncing factor of this mesh.
		float get_bouncing() const;

		/// Returns the number of particles of this mesh.
		size_t size() const;

		/// Returns the type of this mesh. See @ref mt.
		const mesh_type& get_type() const;

		/// Returns a reference to this mesh's particles.
		particles::mesh_particle *get_particles();
		/// Returns a constant reference to this mesh's particles.
		const particles::mesh_particle *get_particles() const;
};

} // -- namespace meshes
} // -- namespace physim
