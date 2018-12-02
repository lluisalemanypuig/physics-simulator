#pragma once

// C++ includes
#include <cstdint>
#include <vector>

// physim includes
#include <physim/initialiser/initialiser.hpp>
#include <physim/geometry/geometry.hpp>
#include <physim/fields/field.hpp>
#include <physim/particles/sized_particle.hpp>
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
		/// The collection of free particles in the simulation.
		std::vector<particles::free_particle *> fps;
		/// The collection of sized particles in the simulation.
		std::vector<particles::sized_particle *> sps;
		/// The collection of meshes in the simulation.
		std::vector<meshes::mesh *> ms;

		/// Gravity of the simulation. [m/s^2].
		math::vec3 gravity;

		/**
		 * @brief Time step of the simulation.
		 *
		 * Default value: 0.01.
		 */
		float dt;
		/**
		 * @brief Viscous drag coefficient.
		 *
		 * Applied as a force in the @ref compute_forces
		 * method to enhance numerical stability (as
		 * suggested in [1]).
		 *
		 * Default value: 0.01.
		 *
		 * [1] Physically based modeling.
		 *     Andrew Witkin (Pixar Animation Studios)
		 *     SIGGRAPH 2001 COURSE NOTES
		 */
		float visc_drag;

		/// Solver used to update each particle's position.
		solver_type solver;

		/**
		 * @brief Initialiser applied to all added particles.
		 *
		 * Whenever @ref add_free_particle(), @ref add_free_particles(size_t),
		 * @ref add_sized_particle() or @ref add_sized_particles()
		 * are called, the attributes of the particles are initialised
		 * using @ref global_init.
		 *
		 * This initialisation is also done whenever the lifetime of a
		 * particle goes below 0. Note that, in this case, some attributes
		 * like the velocity and position may not be zero, as a result
		 * of running the simulation.
		 *
		 * The default behaviour of the function is to not do anything,
		 * that is, the attribtues of the particle are not modified
		 * at all.
		 */
		init::initialiser *global_init;

		/**
		 * @brief Are particle-particle collisions activated?
		 *
		 * Default: no.
		 *
		 * Activates or deactivates the detection of collisions between
		 * pairs of sized particles and between pairs of a free and a
		 * sized particle.
		 *
		 * Notice that this excludes pairs of free particles.
		 */
		bool part_part_collisions;

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
		 * Builds the initial state of the mesh (see @ref meshes::mesh::make_initial_state).
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
		void _simulate_free_particles();
		/**
		 * @brief Simulate sized particles.
		 *
		 * Applies a time step on all the sized particles of
		 * the simulation.
		 */
		void _simulate_sized_particles();

		/**
		 * @brief Simulate meshes.
		 *
		 * Applies a time step on all the particles that make
		 * up the 1-dimensional meshes of the simulation.
		 *
		 * The forces due to the presence of force fields are
		 * computed after the internal forces in each mesh.
		 */
		void _simulate_meshes();

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
		template<class P> void compute_forces(P *p);

		/**
		 * @brief Update a free particle that may collide with geometry.
		 *
		 * A particle has a predicted position and velocity which needs to be
		 * changed in the event that it collides with geometry.
		 * @param[in] p Current state of particle to be updated.
		 * @param[out] pred_pos Predicted position modified to the final position.
		 * @param[out] pred_vel Predicted velocity modified to the final velocity.
		 * @param[out] coll_pred The particle with the updated state.
		 * @returns Returns true on collision with geometry.
		 */
		bool find_update_geom_collision_free
		(
			const particles::free_particle *p,
			math::vec3& pred_pos, math::vec3& pred_vel,
			particles::free_particle& coll_pred
		);

		/**
		 * @brief Update a free particle that may collide with a sized particle.
		 *
		 * A particle has a predicted position and velocity which needs to be
		 * changed in the event that it collides with another particle.
		 * @param[in] p Current state of particle to be updated.
		 * @param[out] pred_pos Predicted position modified to the final position.
		 * @param[out] pred_vel Predicted velocity modified to the final velocity.
		 * @param[out] coll_pred The particle with the updated state.
		 * @returns Returns true on collision with geometry.
		 */
		bool find_update_particle_collision_free(
			const particles::free_particle *p,
			math::vec3& pred_pos, math::vec3& pred_vel,
			particles::free_particle& coll_pred
		);

		/**
		 * @brief Update a sized particle that may collide with geometry.
		 *
		 * A particle has a predicted position and velocity which needs to be
		 * changed in the event that it collides with geometry.
		 *
		 * The difference between this and @ref find_update_geom_collision_free
		 * is that there is an extra intersection test.
		 * @param[in] p Current state of particle to be updated.
		 * @param[out] pred_pos Predicted position modified to the final position.
		 * @param[out] pred_vel Predicted velocity modified to the final velocity.
		 * @param[out] coll_pred The particle with the updated state.
		 * @returns Returns true on collision with geometry.
		 */
		bool find_update_geom_collision_sized
		(
			const particles::sized_particle *p,
			math::vec3& pred_pos, math::vec3& pred_vel,
			particles::sized_particle& coll_pred
		);

		/**
		 * @brief Update a sized particle that may collide with a sized particle.
		 *
		 * This method is called after finding a definitive state of a particle
		 * after colliding with geometry.
		 * @param[in] p Current state of particle to be updated.
		 * @param[in] i Index of the sized particle to ignore.
		 */
		void find_update_particle_collision_sized(particles::sized_particle *p, size_t i);

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

		/**
		 * @brief Adds a free particle to the simulation.
		 *
		 * The particle is initialised using @ref global_init.
		 * @pre For this initialisation to be completely correct, the step
		 * time (see @ref dt) needs to be set.
		 * @post The caller should not free the object, since the simulator
		 * will take care of that.
		 */
		const particles::free_particle *add_free_particle();
		/**
		 * @brief Adds a sized particle to the simulation.
		 *
		 * The particle is initialised using @ref global_init.
		 * @pre For this initialisation to be completely correct, the step
		 * time (see @ref dt) needs to be set.
		 * @post The caller should not free the object, since the simulator
		 * will take care of that.
		 */
		const particles::sized_particle *add_sized_particle();
		/**
		 * @brief Adds the particle passed as parameter to the simulation.
		 *
		 * The initialser function (see @ref global_init) is not called.
		 * The particle is added to @ref fps.
		 * @param p A non-null pointer to the object.
		 * @pre If the solver set to this simulator is @ref solver_type::Verlet,
		 * then the step time (see @ref dt) needs to be set to initialise correctly
		 * the particle's previous position.
		 * @post The caller should not free the object, since the simulator
		 * will take care of that.
		 */
		void add_free_particle(particles::free_particle *p);
		/**
		 * @brief Adds the particle passed as parameter to the simulation.
		 *
		 * The initialser function (see @ref global_init) is not called.
		 * The particle is added to @ref sps.
		 *
		 * @param p A non-null pointer to the object.
		 * @pre If the solver set to this simulator is @ref solver_type::Verlet,
		 * then the step time (see @ref dt) needs to be set to initialise correctly
		 * the particle's previous position.
		 * @post The caller should not free the object, since the simulator
		 * will take care of that.
		 */
		void add_sized_particle(particles::sized_particle *p);
		/**
		 * @brief Adds @e n free particles to the simulation.
		 *
		 * Each of the particles is initialised with the @ref global_init
		 * object used at the moment of calling this method. The particle
		 * is added to @ref fps.
		 * @param n Number of particles.
		 */
		void add_free_particles(size_t n);
		/**
		 * @brief Adds @e n sized particles to the simulation.
		 *
		 * Each of the particles is initialised with the @ref global_init
		 * object used at the moment of calling this method. The particle
		 * is added to @ref sps.
		 * @param n Number of particles.
		 */
		void add_sized_particles(size_t n);

		/**
		 * @brief Removes the @e i-th free particle.
		 *
		 * Frees the memory occupied by the particle in the @e i-th
		 * position of @ref fps. Therefore, any pointer to that particle
		 * becomes invalid.
		 * @param i The index of the particle in [0, number of particles).
		 */
		void remove_free_particle(size_t i);
		/**
		 * @brief Removes the @e i-th sized particle.
		 *
		 * Frees the memory occupied by the particle in the @e i-th
		 * position of @ref sps. Therefore, any pointer to that particle
		 * becomes invalid.
		 * @param i The index of the particle in [0, number of particles).
		 */
		void remove_sized_particle(size_t i);

		/**
		 * @brief Deletes all free particles in this simulator.
		 *
		 * Deletes all the objects in @ref fps and clears the
		 * container.
		 */
		void clear_free_particles();
		/**
		 * @brief Deletes all sized particles in this simulator.
		 *
		 * Deletes all the objects in @ref sps and clears the
		 * container.
		 */
		void clear_sized_particles();
		/**
		 * @brief Deletes all particles in this simulator.
		 *
		 * Deletes all the objects in @ref fps and @ref sps and clears
		 * the containers.
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
		 * @brief Simulate free particles.
		 *
		 * Calls @ref _simulate_free_particles and increments the time
		 * of the simulation (see @ref stime).
		 */
		void simulate_free_particles();
		/**
		 * @brief Simulate sized particles.
		 *
		 * Calls @ref _simulate_sized_particles and increments the time
		 * of the simulation (see @ref stime).
		 */
		void simulate_sized_particles();

		/**
		 * @brief Simulate meshes.
		 *
		 * Calls @ref _simulate_meshes and increments the time of
		 * the simulation (see @ref stime).
		 */
		void simulate_meshes();

		/**
		 * @brief Apply a time step to the simulation.
		 *
		 * Calls the following functions:
		 * - @ref _simulate_free_particles
		 * - @ref _simulate_sized_particles
		 * - @ref _simulate_meshes
		 * and increments the time of the simulation. Parameter
		 * @e dt (set via method @ref set_time_step(float)) indicates
		 * how much time has passed since the last time step.
		 */
		void apply_time_step();

		// SETTERS

		/**
		 * @brief Sets the time step of the simulation.
		 * @param t Time step of the simulation.
		 */
		void set_time_step(float t);

		/**
		 * @brief Sets the viscous drag coefficient.
		 * @param d Positive floating-point value.
		 */
		void set_viscous_drag(float d);

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

		/**
		 * @brief Activates/Deactivates particle-particle collisions.
		 *
		 * Sets the value to @ref part_part_collisions.
		 * @param a Either true or false.
		 */
		void set_particle_particle_collisions(bool a);

		// GETTERS

		/**
		 * @brief Returns all free particles in the simulation.
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
		const std::vector<particles::free_particle *>& get_free_particles() const;
		/// Returns a constant reference to i-th free particle.
		const particles::free_particle& get_free_particle(size_t i) const;

		/**
		 * @brief Returns all sized particles in the simulation.
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
		const std::vector<particles::sized_particle *>& get_sized_particles() const;
		/// Returns a constant reference to i-th sized particle.
		const particles::sized_particle& get_sized_particle(size_t i) const;

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

		/// Returns the time step of the simulation (see @ref dt).
		float get_time_step() const;

		/**
		 * @brief Are collisions between particles activated?
		 *
		 * See @ref part_part_collisions for details.
		 * @return Returns the value of @ref part_part_collisions.
		 */
		bool part_part_colls_activated() const;
};

} // -- namespace sim
