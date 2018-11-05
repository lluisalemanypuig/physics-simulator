#pragma once

// C++ includes
#include <cstdint>
#include <vector>

// physim includes
#include <physim/initialiser/initialiser.hpp>
#include <physim/geometry/geometry.hpp>
#include <physim/fields/field.hpp>
#include <physim/particles/free_particle.hpp>
#include <physim/meshes/mesh.hpp>

namespace physim {

/**
 * @brief The different types of solvers.
 *
 * Each solver updates the position and velocity
 * of a particle differently:
 * - EulerOrig: see @ref solver_type::EulerOrig.
 * - EulerSemi: see @ref solver_type::EulerSemi.
 * - Verlet: see @ref solver_type::Verlet.
 */
enum class solver_type : int8_t {
	none = -1,

	/**
	 * Implements the usual version of the Euler solver.
	 * Position and velocity are updated with the usual formulas, and
	 * in this order.
	 \verbatim
	 xc := current position
	 new position = xc + time step * current velocity
	 new volcity = xc + time step * force / particle mass
	 \endverbatim
	 */
	EulerOrig,

	/**
	 * implements a slightly different version from the
	 * @e EulerOrig solver. Although it uses the same formulas,
	 * the velocity is updated first, and then the position is updated.
	 */
	EulerSemi,

	/**
	 * This solver does not use the particle's velocity to update
	 * its position. However, the velocity is still updated.
	 \verbatim
	 xc := current position
	 xp := previous position
	 new position = xc + k*(xc - xp) + force * time step * force / particle mass
	 new velocity = (new position - xc)/time step
	 \endverbatim
	 */
	Verlet
};

/**
 * @brief Simulator class.
 *
 * This class contains the algorihm used to simulate the
 * movement of particles and systems of particles (like
 * springs systems).
 *
 * Use method @ref apply_time_step to run the simulation
 * step by step.
 *
 * Use method @ref add_geometry to add fixed geometrical
 * objects (spheres, triangles, planes) with which the
 * particles may collide with.
 *
 * Function @ref reset_simulation resets all particle to
 * their original state using the initialiser function
 * (see @ref global_init).
 */
class simulator {
	private:
		/**
		 * @brief The collection of objects whose position
		 * does not depend on the interaction with other objects.
		 */
		std::vector<geom::geometry *> scene_fixed;
		/// Collection of force fields.
		std::vector<fields::field *> force_fields;
		/// The collection of particles in the simulation.
		std::vector<particles::free_particle *> ps;
		/// The collection of meshes in the simulation.
		std::vector<meshes::mesh *> ms;

		/// Gravity of the simulation. [m/s^2].
		math::vec3 gravity;

		/// Current time of the simulation.
		float stime;
		/// Time step of the simulation.
		float dt;

		/// Solver used to update each particle's position.
		solver_type solver;

		/**
		 * @brief Initialiser applied to all added particles.
		 *
		 * Whenever @ref add_particle() or @ref add_particles(size_t)
		 * are called, the attributes of the particles are initialised
		 * using @ref global_init.
		 *
		 * This initialisation is also done whenever the lifetime of a
		 * particle goes below 0. Note that, in this case, some attributes
		 * like the velocity and position may not be zero, as a result
		 * of running the simulation.
		 *
		 * The default behaviour of the function is not to do anything,
		 * that is, the attribtues of the particle are not modified
		 * at all.
		 *
		 * It is important to mention that the force applied on a particle
		 * is always set to be equal to the simulator's gravity (see @ref
		 * gravity). However, it is set before calling the initialiser
		 * function. Therefore, it is recommended to use the method @ref
		 * particle::add_force if it is to be modified.
		 *
		 * It is recommended to set the particle's 'previous' state
		 * to null values.
		 */
		init::initialiser *global_init;

	private:

		/**
		 * @brief Initialises a particle using @ref global_init and computes
		 * an initial previous position.
		 *
		 * Calls @ref global_init to initialise the rest of its
		 * attributes and updates its previous position so that the Verlet
		 * solver can corectly (see @ref solver_type).
		 * @param p The particle to be initialsed.
		 */
		void init_particle(particles::free_particle *p);

		/**
		 * @brief Initialises a mesh.
		 *
		 * Computes an initial previous position for each particle of
		 * the mesh so that the Verlet solver can work correctly
		 * (see @ref solver_type).
		 * @param m The mesh to be initialsed.
		 */
		void init_mesh(meshes::mesh *m);

		/**
		 * @brief Simulate free particles.
		 *
		 * Applies a time step on all the free particles of
		 * the simulation.
		 */
		void simulate_free_particles();

		/**
		 * @brief Simulate meshes.
		 *
		 * Applies a time step on all the particles that make
		 * up the 1-dimensional meshes of the simulation.
		 *
		 * The forces due to the presence of force fields are
		 * computed after the internal forces in each mesh.
		 */
		void simulate_meshes();

		/**
		 * @brief Predicts a particle's next position and velocity.
		 * @param p Particle to apply the solver on.
		 * @param[out] pos The predicted position.
		 * @param[out] vel The predicted velocity.
		 */
		template<class P> void apply_solver
		(const P *p, math::vec3& pos, math::vec3& vel);

		/**
		 * @brief Computes the forces acting in the simulation.
		 *
		 * These forces are defined in the different focuses added
		 * to the simulator. Each force vector is added to the particle's
		 * force accumulator. The force accumulator, however, is not
		 * initialised to 0.
		 *
		 * The result is set to the force acting on particle @e p.
		 * @param[out] p The particle whose force attribute is to be modified.
		 */
		template<class P>
		void compute_forces(P *p);

	public:
		/**
		 * @brief Default constructor.
		 * @param s Numerical solver to move the particles.
		 * @param t Time step of the simulation.
		 */
		simulator(const solver_type& s = solver_type::EulerSemi, float t = 0.01f);
		/// Destructor
		~simulator();

		// BUILD SIMULATION'S CONTENTS

		// ----------- particles

		/// Adds a particle to the simulation.
		const particles::free_particle *add_particle();
		/**
		 * @brief Adds the particle passed as parameter to the simulation.
		 *
		 * The initialser function (see @ref global_init) is not called.
		 * For this initialisation to be completely correct, the step time
		 * (see @ref dt) needs to be set.
		 *
		 * The caller should not free the object, since the simulator
		 * will take care of that.
		 * @param p A non-null pointer to the object.
		 */
		void add_particle(particles::free_particle *p);
		/// Adds @e n particles to the simulation.
		void add_particles(size_t n);

		/**
		 * @brief Removes the @e i-th particle.
		 *
		 * Frees the memory occupied by the particle in the @e i-th
		 * position. Therefore, any pointer to that particle becomes
		 * invalid.
		 * @param i The index of the particle in [0, number of particles).
		 */
		void remove_particle(size_t i);
		/**
		 * @brief Deletes all particles in this simulator.
		 *
		 * Deletes all the objects in @ref ps and clears the
		 * container.
		 */
		void clear_particles();

		// ----------- geometry

		/**
		 * @brief Adds a geometrical object to the scene.
		 *
		 * The geometrical object is added to @ref scene_fixed.
		 *
		 * The caller should not free the object since the simulator
		 * will take care of that.
		 * @param g A non-null pointer to the object.
		 */
		void add_geometry(geom::geometry *g);
		/**
		 * @brief Removes the @e i-th fixed geometrical object.
		 *
		 * Frees the memory occupied by the object in the @e i-th position from
		 * @ref scene_fixed. Therefore, any pointer to that object becomes invalid.
		 * @param i The index of the object in [0, number of geometrical objects).
		 */
		void remove_geometry(size_t i);
		/**
		 * @brief Deletes all geometry in this simulator.
		 *
		 * Deletes all the objects in @ref scene_fixed and clears
		 * the container.
		 */
		void clear_geometry();

		// ----------- fields

		/**
		 * @brief Adds a force field to the scene.
		 *
		 * The force field is added to @ref force_fields.
		 *
		 * The caller should not free the object since the simulator
		 * will take care of that.
		 * @param f A non-null pointer to the object.
		 */
		void add_field(fields::field *f);
		/**
		 * @brief Adds a gravity vector.
		 *
		 * Makes a force field of type @ref fields::gravitational_planet
		 * using vector @e g.
		 */
		void add_gravity_acceleration(const math::vec3& g);
		/**
		 * @brief Removes the @e i-th force field object.
		 *
		 * Frees the memory occupied by the object in the @e i-th position from
		 * @ref force_fields. Therefore, any pointer to that object becomes invalid.
		 * @param i The index of the object in [0, number of force fields).
		 */
		void remove_field(size_t i);
		/**
		 * @brief Deletes all force fields in this simulator.
		 *
		 * Deletes all the objects in @ref force_fields and clears
		 * the container.
		 */
		void clear_fields();

		// ----------- meshes

		/**
		 * @brief Adds a mesh to the scene.
		 *
		 * The mesh is added to @ref ms.
		 *
		 * The caller should not free the object since the simulator
		 * will take care of that.
		 *
		 * This call must be called after initialising the mesh (that is,
		 * after initialising the positions and making its initial state).
		 * @param m A non-null pointer to the object.
		 */
		void add_mesh(meshes::mesh *m);
		/**
		 * @brief Removes the @e i-th mesh object.
		 *
		 * Frees the memory occupied by the object in the @e i-th position from
		 * @ref ms. Therefore, any pointer to that object becomes invalid.
		 * @param i The index of the object in [0, number of meshes).
		 */
		void remove_mesh(size_t i);
		/**
		 * @brief Deletes all meshes in this simulator.
		 *
		 * Deletes all the objects in @ref ms and clears the container.
		 */
		void clear_meshes();

		// MODIFIERS

		/**
		 * @brief Clears the simulation to an empty state;
		 *
		 * Deletes all particles, geometry and resets the simulation time.
		 * 
		 * That is, calls:
		 * - @ref clear_particles,
		 * - @ref clear_geometry,
		 * - @ref clear_fields,
		 * - @ref clear_meshes,
		 * and sets the simulation time (@ref stime) to 0.
		 */
		void clear_simulation();

		/**
		 * @brief Resets the simulation to its initial state.
		 *
		 * Resets the simulation time (see @ref stime) to 0 and
		 * initialises all particles with the initialiser function
		 * @ref global_init.
		 */
		void reset_simulation();

		// RUN SIMULATION

		/**
		 * @brief Apply a time step to the simulation.
		 *
		 * Particles move according to time. Parameter @e dt
		 * (set via method @ref set_time_step(float)) indicates
		 * how much time has passed since the last time step.
		 *
		 * Whenever a particle's lifetime has reached 0, method
		 * @ref init_particle(particle*) is called.
		 */
		void apply_time_step();

		// SETTERS

		/**
		 * @brief Sets the time step of the simulation.
		 * @param t Time step of the simulation.
		 */
		void set_time_step(float t);

		/**
		 * @brief Sets the particle initialiser function.
		 *
		 * The simulator copies the object, therefore the intialiser
		 * object should be freed by the user. Since the simulator
		 * keeps its own copy, the parameter may be freed any time.
		 *
		 * The previous initialiser is destroyed by the class.
		 * @param f This function need not return any value. The
		 * pointer passed as parameter is always guaranteed to
		 * be non-null.\n
		 * This function will be called the first time the lifetime
		 * of the particle has reached a value equal to or smaller
		 * than 0.\n
		 * The parameter cannot be null.
		 */
		void set_initialiser(const init::initialiser *f);

		/**
		 * @brief Sets the type of solver.
		 *
		 * There are several types of solvers.
		 * See enumeration @ref solver_type.
		 * @param s The type of sovler to be used.
		 */
		void set_solver(const solver_type& s);

		// GETTERS

		/**
		 * @brief Returns all particles in the simulation.
		 *
		 * Note that the constant reference is to the container.
		 * It cannot be added new particles or have deleted any,
		 * however any particle's attributes may be modified.
		 *
		 * The behaviour of the modified particles in the simulation
		 * will change according to the modifications.
		 * @return Returns a constant reference to the structure
		 * containing all particles.
		 */
		const std::vector<particles::free_particle *>& get_particles() const;
		/// Returns a constant reference to i-th particle.
		const particles::free_particle& get_particle(size_t i) const;
		/**
		 * @brief Returns all meshes in the simulation.
		 *
		 * Note that the constant reference is to the container.
		 * It cannot be added new meshes or have deleted any,
		 * however any meshe's attributes may be modified.
		 *
		 * The behaviour of the modified particles in the simulation
		 * will change according to the modifications.
		 * @return Returns a constant reference to the structure
		 * containing all particles.
		 */
		const std::vector<meshes::mesh *>& get_meshes() const;
		/// Returns a constant reference to i-th mesh.
		const meshes::mesh& get_mesh(size_t i) const;
		/// Returns all fixed objects of the scene.
		const std::vector<geom::geometry *>& get_fixed_objects() const;
		/// Returns the current simulation time.
		float get_current_time() const;
		/// Returns the gravity of the scene.
		const math::vec3& get_gravity() const;
		/// Returns the number of particles.
		size_t n_particles() const;
		/// Returns the number of fixed geometrical objects.
		size_t n_geometry() const;
		/// Returns the initialiser functions.
		init::initialiser *get_initialiser();
		/// Returns a constant reference to the initialiser functions.
		const init::initialiser *get_initialiser() const;
};

} // -- namespace sim
