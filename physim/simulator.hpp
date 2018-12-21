#pragma once

// C++ includes
#include <cstdint>
#include <vector>

// physim includes
#include <physim/emitter/free_emitter.hpp>
#include <physim/emitter/sized_emitter.hpp>
//#include <physim/emitter/emitter_agent.hpp>
#include <physim/geometry/geometry.hpp>
#include <physim/fields/field.hpp>
#include <physim/particles/sized_particle.hpp>
#include <physim/particles/agent_particle.hpp>
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
	 new position = xc + k*(xc - xp) + force * dt^2 / mass
	 new velocity = (new position - xc) / dt
	 \endverbatim
	 * where
	 * - xc    :: current position
	 * - xp    :: previous position
	 * - k     :: damping factor (set to 1.0)
	 * - force :: the force that acts upon the particle
	 * - dt    :: time step
	 * - mass  :: particle's mass
	 */
	Verlet
};

/**
 * @brief Simulator class.
 *
 * This class contains the algorihms used to simulate the
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
 * their original state using the initialiser objects
 * (see @ref free_global_emit, @ref sized_global_emit).
 */
class simulator {
	private:
		/**
		 * @brief Collection of fixed geometry.
		 *
		 * The collection of objects whose position
		 * does not depend on the interaction with other objects.
		 * This position is, then, fixed.
		 */
		std::vector<geometric::geometry *> scene_fixed;
		/// Collection of force fields.
		std::vector<fields::field *> force_fields;
		/// The collection of free particles in the simulation.
		std::vector<particles::free_particle *> fps;
		/// The collection of sized particles in the simulation.
		std::vector<particles::sized_particle *> sps;
		/// The collection of agent particles in the simulation.
		std::vector<particles::agent_particle *> aps;
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
		 * @brief Initialiser applied to all free particles.
		 *
		 * Whenever @ref add_free_particle() or @ref add_free_particles(size_t)
		 * are called, the attributes of the particles are initialised using
		 * this object.
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
		emitters::free_emitter *free_global_emit;
		/**
		 * @brief Initialiser applied to all sized particles.
		 *
		 * Whenever @ref add_sized_particle() or @ref add_sized_particles(size_t)
		 * are called, the attributes of the particles are initialised using
		 * this object.
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
		emitters::sized_emitter *sized_global_emit;

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
		 * @brief Initialises a particle using its corresponding emitter.
		 *
		 * Calls one of
		 * - @ref free_global_emit
		 * - @ref sized_global_emit
		 * to initialise its attributes, according to the type of particle.
		 *
		 * This function also updates the particle's previous position
		 * so that the Verlet solver can be correctly used (see
		 * @ref solver_type::Verlet).
		 * @param p The particle to be initialsed.
		 */
		void init_particle(particles::base_particle *p);

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
		 * @brief Simulate agent particles.
		 *
		 * Applies a time step on all the agent particles of
		 * the simulation.
		 */
		void _simulate_agent_particles();

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
		bool find_update_geomcoll_free
		(
			const particles::free_particle *p,
			math::vec3& pred_pos, math::vec3& pred_vel,
			particles::free_particle& coll_pred
		);

		/**
		 * @brief Update a free particle that may collide with a sized
		 * or an agent particle.
		 *
		 * A particle has a predicted position and velocity which needs to be
		 * changed in the event that it collides with another particle.
		 * @param[in] p Current state of particle to be updated.
		 * @param[out] pred_pos Predicted position modified to the final position.
		 * @param[out] pred_vel Predicted velocity modified to the final velocity.
		 * @param[out] coll_pred The particle with the updated state.
		 * @returns Returns true on collision with geometry.
		 */
		bool find_update_partcoll_free(
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
		 * The difference between this and @ref find_update_partcoll_free
		 * is that there is an extra intersection test.
		 * @param[in] p Current state of particle to be updated.
		 * @param[out] pred_pos Predicted position modified to the final position.
		 * @param[out] pred_vel Predicted velocity modified to the final velocity.
		 * @param[out] coll_pred The particle with the updated state.
		 * @returns Returns true on collision with geometry.
		 */
		bool find_update_geomcoll_sized
		(
			const particles::sized_particle *p,
			math::vec3& pred_pos, math::vec3& pred_vel,
			particles::sized_particle& coll_pred
		);

		/**
		 * @brief Update a sized particle that may collide with a sized
		 * or an agent particle.
		 *
		 * When checking collisions with sized particles, the @e i-th particle
		 * is ignored.
		 *
		 * @param[in] p Current state of particle to be updated.
		 * @param[in] i Index of the sized particle to ignore.
		 * @pre This method is called after finding a definitive state of a
		 * particle after colliding with geometry.
		 */
		void find_update_partcoll_sized
		(particles::sized_particle *p, size_t i);

		/**
		 * @brief Update an agent particle that may collide with a sized or an
		 * agent particle.
		 *
		 * When checking collisions with agent particles, the @e i-th particle
		 * is ignored.
		 *
		 * @param[in] p Current state of particle to be updated.
		 * @param[in] i Index of the sized particle to ignore.
		 * @pre This method is called after finding a definitive state of a
		 * particle after colliding with geometry.
		 */
		void find_update_partcoll_agent
		(particles::agent_particle *p, size_t i);

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
		 * The particle is initialised using @ref free_global_emit.
		 * @pre For this initialisation to be completely correct, the step
		 * time (see @ref dt) needs to be set.
		 * @post The caller should not free the object, since the simulator
		 * will take care of that.
		 */
		particles::free_particle *add_free_particle();
		/**
		 * @brief Adds a sized particle to the simulation.
		 *
		 * The particle is initialised using @ref sized_global_emit.
		 * @pre For this initialisation to be completely correct, the step
		 * time (see @ref dt) needs to be set.
		 * @post The caller should not free the object, since the simulator
		 * will take care of that.
		 */
		particles::sized_particle *add_sized_particle();
		/**
		 * @brief Adds the particle passed as parameter to the simulation.
		 *
		 * The initialser function (see @ref free_global_emit) is not called.
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
		 * The initialiser object (see @ref sized_global_emit) is not called.
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
		 * @brief Adds the particle passed as parameter to the simulation.
		 *
		 * None of the initialiser objects are used. The particle is added
		 * to @ref aps.
		 *
		 * @param p A non-null pointer to the object.
		 * @pre If the solver set to this simulator is @ref solver_type::Verlet,
		 * then the step time (see @ref dt) needs to be set to initialise correctly
		 * the particle's previous position.
		 * @post The caller should not free the object, since the simulator
		 * will take care of that.
		 */
		void add_agent_particle(particles::agent_particle *p);
		/**
		 * @brief Adds @e n free particles to the simulation.
		 *
		 * Each of the particles is initialised with the @ref free_global_emit
		 * object used at the moment of calling this method. The particle
		 * is added to @ref fps.
		 * @param n Number of particles.
		 */
		void add_free_particles(size_t n);
		/**
		 * @brief Adds @e n sized particles to the simulation.
		 *
		 * Each of the particles is initialised with the @ref sized_global_emit
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
		 * @param i The index of the particle in [0, number of free particles).
		 */
		void remove_free_particle(size_t i);
		/**
		 * @brief Removes the @e i-th sized particle.
		 *
		 * Frees the memory occupied by the particle in the @e i-th
		 * position of @ref sps. Therefore, any pointer to that particle
		 * becomes invalid.
		 * @param i The index of the particle in [0, number of sized particles).
		 */
		void remove_sized_particle(size_t i);
		/**
		 * @brief Removes the @e i-th agent particle.
		 *
		 * Frees the memory occupied by the particle in the @e i-th
		 * position of @ref aps. Therefore, any pointer to that particle
		 * becomes invalid.
		 * @param i The index of the particle in [0, number of agent particles).
		 */
		void remove_agent_particle(size_t i);

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
		 * @brief Deletes all agent particles in this simulator.
		 *
		 * Deletes all the objects in @ref aps and clears the
		 * container.
		 */
		void clear_agent_particles();
		/**
		 * @brief Deletes all particles in this simulator.
		 *
		 * Deletes all the objects in @ref fps, @ref sps and @ref aps,
		 * and clears the containers.
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
		void add_geometry(geometric::geometry *g);
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
		 * @brief Sets the gravity vector.
		 *
		 * Makes a force field of type @ref fields::gravitational_planet
		 * using vector @e g.
		 */
		void set_gravity_acceleration(const math::vec3& g);
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
		 * - @ref clear_meshes.
		 */
		void clear_simulation();

		/**
		 * @brief Resets the simulation to its initial state.
		 *
		 * Initialises all particles with the initialiser objects
		 * (using the appropriate objects for each type of particle).
		 */
		void reset_simulation();

		// RUN SIMULATION

		/**
		 * @brief Simulate free particles.
		 *
		 * Calls @ref _simulate_free_particles.
		 *
		 * A free particle will only start 'living', i.e., moving in the
		 * environment, when its starting time (see @ref free_particle::starttime)
		 * is equal or less than 0. Also, as long as @ref free_particle::fixed
		 * is set to false. At every time step, the starting time of each particle
		 * is decreased by an amount equal to the time step (see @ref dt).
		 *
		 * The lifetime of each particle (see @ref free_particle::lifetime) is
		 * also decreased by the exact same amount. Whenever this value reaches
		 * 0 the particle is reinitialised using the emitter for free particles
		 * (see @ref free_global_emit).
		 *
		 * Free particles collide with the geometrical objects added via method
		 * @ref add_geometry(geometric::geometry *), and are moved depending on
		 * that geometry.
		 *
		 * When activated, collisions of free particles against sized particles
		 * are checked.
		 */
		void simulate_free_particles();
		/**
		 * @brief Simulate sized particles.
		 *
		 * Calls @ref _simulate_sized_particles.
		 *
		 * The simulation of sized particles follows exactly the same rules as
		 * those for the simulation of free particles. The only difference is
		 * that collisions with geometrical object are copmuted considering
		 * that these particles have a certain size (see @ref sized_particle::R).
		 *
		 * When activated, collisions of free particles against sized particles
		 * are checked.
		 */
		void simulate_sized_particles();
		/**
		 * @brief Simulate agent particles.
		 *
		 * Calls @ref _simulate_agent_particles.
		 *
		 * That function applies the steering behaviour set to every agent
		 * particle in its attribute @ref agent_particle::behaviour.
		 *
		 * The simulation of an agent particle, regarding collisions with
		 * geometry, if any, is done similarly as in the simulation of sized
		 * particles.
		 */
		void simulate_agent_particles();

		/**
		 * @brief Simulate meshes.
		 *
		 * Calls @ref _simulate_meshes.
		 *
		 * Each mesh has each of its particle's force updated according to
		 * the definition provided in method @ref meshes::mesh::update_forces. This
		 * method defines the model implemented in that mesh. Then, each mesh
		 * is considered as a free particle and simulated accordingly.
		 */
		void simulate_meshes();

		/**
		 * @brief Apply a time step to the simulation.
		 *
		 * Calls the following functions:
		 * - @ref _simulate_sized_particles
		 * - @ref _simulate_agent_particles.
		 * - @ref _simulate_free_particles
		 * - @ref _simulate_meshes.
		 * Parameter @e dt (set via method @ref set_time_step(float))
		 * indicates how much time has passed since the last time step.
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
		 * @brief Sets the free particle emitter object.
		 *
		 * See @ref free_global_emit.
		 *
		 * The simulator copies the object, therefore the intialiser
		 * object should be freed by the user. Since the simulator
		 * keeps its own copy, the parameter may be freed any time.
		 *
		 * The previous emitter is destroyed by the class.
		 * @param f Free particle emitter object.
		 * @pre @e f can not be null.
		 */
		void set_free_emitter(const emitters::free_emitter *f);
		/**
		 * @brief Sets the sized particle emitter object.
		 *
		 * See @ref sized_global_emit.
		 *
		 * The simulator copies the object, therefore the intialiser
		 * object should be freed by the user. Since the simulator
		 * keeps its own copy, the parameter may be freed any time.
		 *
		 * The previous emitter is destroyed by the class.
		 * @param s Sized particle emitter object.
		 * @pre @e s can not be null.
		 */
		void set_sized_emitter(const emitters::sized_emitter *s);

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
		 * containing all free particles.
		 */
		const std::vector<particles::free_particle *>& get_free_particles() const;
		/// Returns a reference to i-th free particle.
		particles::free_particle& get_free_particle(size_t i);
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
		 * containing all sized particles.
		 */
		const std::vector<particles::sized_particle *>& get_sized_particles() const;
		/// Returns a reference to i-th sized particle.
		particles::sized_particle& get_sized_particle(size_t i);
		/// Returns a constant reference to i-th sized particle.
		const particles::sized_particle& get_sized_particle(size_t i) const;

		/**
		 * @brief Returns all agent particles in the simulation.
		 *
		 * Note that the constant reference is to the container.
		 * It cannot be added new particles or have deleted any,
		 * however any particle's attributes may be modified.
		 *
		 * The behaviour of the modified particles in the simulation
		 * will change according to the modifications.
		 * @return Returns a constant reference to the structure
		 * containing all agent particles.
		 */
		const std::vector<particles::agent_particle *>& get_agent_particles() const;
		/// Returns a reference to i-th agent particle.
		particles::agent_particle& get_agent_particle(size_t i);
		/// Returns a constant reference to i-th agent particle.
		const particles::agent_particle& get_agent_particle(size_t i) const;

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
		const std::vector<geometric::geometry *>& get_fixed_objects() const;
		/// Returns the gravity of the scene.
		const math::vec3& get_gravity() const;
		/// Returns the number of particles.
		size_t n_particles() const;
		/// Returns the number of fixed geometrical objects.
		size_t n_geometry() const;
		/// Returns the free emitter object.
		emitters::free_emitter *get_free_emitter();
		/// Returns a constant reference to the free emitter object.
		const emitters::free_emitter *get_free_emitter() const;
		/// Returns the sized emitter object.
		emitters::sized_emitter *get_sized_emitter();
		/// Returns a constant reference to the sized emitter object.
		const emitters::sized_emitter *get_sized_emitter() const;

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
