#pragma once

// C inlcudes
#include <assert.h>

// glm includes
#include <glm/glm.hpp>
using namespace glm;

namespace physim {

/**
 * @brief Class implementing a particle.
 *
 * A particle is a 0-dimensional object, subject to
 * several forces. It can also collide with other
 * objects in the scene (geometrical objects, see
 * namespace @ref physim::geom) but not with other
 * particles.
 */
class particle {
	private:
		/// Previous position of the particle.
		vec3 prev_pos;
		/// Current position of the particle.
		vec3 cur_pos;
		/// Prevous velocity of the particle.
		vec3 prev_velocity;
		/// Current velocity of the particle.
		vec3 cur_velocity;
		/// Force currently applied to the particle.
		vec3 force;

		/// Friction coefficient of the particle.
		float friction;
		/// Bouncing coefficient of the particle.
		float bouncing;
		/**
		 * @brief Lifetime of the particle.
		 *
		 * Once the simulation has run for a time larger than
		 * @ref lifetime the particle must be reset.
		 */
		float lifetime;
		/**
		 * @brief Starting time of a particle.
		 *
		 * The amount of time that has to pass for this particle
		 * to start 'living'. In the simulation, this particle
		 * will not start moving until this value is equal to or
		 * less than 0.
		 */
		float starttime;
		/**
		 * @brief Is this particle fixed?
		 *
		 * If the particle, it will be ignored by the solver, therefore
		 * not taken into account in the simulation (gravity nor any
		 * other force will have any effect on it).
		 */
		bool fixed;

		/**
		 * @brief Index of the particle.
		 *
		 * This index is automatically set when added to the
		 * simulator object. The collection of indexes determine
		 * the order in which particles have been added to it.
		 * The indexes start at 0.
		 *
		 * It can be used to initialise its attributes through
		 * the @ref init::initialiser class.
		 */
		size_t index;

	private:

		/**
		 * @brief Initialises all particle's attributes, most of them
		 * to null values.
		 *
		 * The attributes of the class take the following values:
		 * - @ref prev_pos : vec3(0,0,0)
		 * - @ref cur_pos : no value assigned to it, since it will
		 * be overwritten later.
		 * - @ref prev_velocity : vec3(0,0,0)
		 * - @ref cur_velocity : vec3(0,0,0)
		 * - @ref force : vec3(0,0,0)
		 * - @ref friction : 0
		 * - @ref bouncing : 1
		 * - @ref lifetime : 10
		 * - @ref starttime : 0
		 * - @ref fixed : false
		 * - @ref index : no value assigned, since it will be
		 * overwritten by the simulator.
		 */
		void init();

	public:
		/// Default constructor.
		particle();
		/// Construct particle with position as 3 float values.
		particle(const float& x, const float& y, const float& z);
		/// Construct particle with position as a vec3.
		particle(const vec3& cur_pos);
		/// Copy constructor.
		particle(const particle& p);
		/// Destructor.
		~particle();

		// MODIFIERS

		/**
		 * @brief Translate the particle according to vector @e v.
		 *
		 * Increment current position (see @ref cur_pos) by @e v.
		 * @param v The direction of the translation.
		 */
		void translate(const vec3& v);
		/**
		 * @brief Increment the velocity of the particle.
		 *
		 * Increments the velocity of the particle (see @ref cur_velocity)
		 * by @e v.
		 * @param v The direction of the acceleration.
		 */
		void acceleterate(const vec3& v);

		/**
		 * @brief Adds @e (x,y,z) to the particle's @ref force.
		 * @post @ref force += @e (x,y,z)
		 */
		void add_force(const float& x, const float& y, const float& z);
		/**
		 * @brief Adds @e f to the particle's @ref force.
		 * @post @ref force += @e f
		 */
		void add_force(const vec3& f);

		/**
		 * @brief Decreases the lifetime by @e t.
		 *
		 * @param t A value equal to or greater than 0.
		 */
		void reduce_lifetime(float t);
		/**
		 * @brief Decreases the starttime by @e t.
		 *
		 * @param t A value equal to or greater than 0.
		 */
		void reduce_starttime(float t);

		/**
		 * @brief Saves the current position in the particle's state.
		 *
		 * Copies @ref cur_pos into @ref prev_pos.
		 */
		void save_position();
		/**
		 * @brief Saves the current velocity in the particle's state.
		 *
		 * Copies @ref cur_velocity into @ref prev_velocity.
		 */
		void save_velocity();

		// SETTERS

		/// Sets the previous position of the particle. See @ref prev_pos.
		void set_previous_position(const float& x, const float& y, const float& z);
		/// Sets the previous position of the particle. See @ref prev_pos.
		void set_previous_position(const vec3& cur_pos);

		/// Sets the current position of the particle. See @ref cur_pos.
		void set_position(const float& x, const float& y, const float& z);
		/// Sets the current position of the particle. See @ref cur_pos.
		void set_position(const vec3& cur_pos);

		/// Sets the previous velocity of the particle. See @ref prev_velocity.
		void set_previous_velocity(const float& x, const float& y, const float& z);
		/// Sets the previous velocity of the particle. See @ref prev_velocity.
		void set_previous_velocity(const vec3& vel);

		/// Sets the current velocity of the particle. See @ref cur_velocity.
		void set_velocity(const float& x, const float& y, const float& z);
		/// Sets the current velocity of the particle. See @ref cur_velocity.
		void set_velocity(const vec3& vel);

		/// Sets the force of the particle. See @ref force.
		void set_force(const float& x, const float& y, const float& z);
		/// Sets the force of the particle. See @ref force.
		void set_force(const vec3& f);

		/// Sets the friction coefficient of the particle. See @ref friction.
		void set_friction(float f);
		/// Sets the bouncing coefficient of the particle. See @ref bouncing.
		void set_bouncing(float b);
		/// Sets the lifetime of the particle. See @ref lifetime.
		void set_lifetime(float lT);
		/// Sets the starttime of the particle. See @ref starttime.
		void set_starttime(float sT);
		/// Sets whether this particle is fixed or not. See @ref fixed.
		void set_fixed(bool f);

		/// Sets the index of the particle. See @ref index.
		void set_index(size_t i);

		// GETTERS

		/// Returns the previous position. See @ref prev_pos.
		const vec3& get_previous_position() const;
		/// Returns the current position. See @ref cur_pos.
		const vec3& get_position() const;
		/// Returns the previous velocity of the particle. See @ref prev_velocity.
		const vec3& get_previous_velocity() const;
		/// Returns the current velocity of the particle. See @ref cur_velocity.
		const vec3& get_velocity() const;
		/// Returns the force applied to the particle. See @ref force.
		const vec3& get_force() const;
		/// Returns the friction coefficient of the particle. See @ref friction.
		float get_friction() const;
		/// Returns the bouncing coefficient of the particle. See @ref bouncing.
		float get_bouncing() const;
		/// Returns the lifetime of the particle. See @ref lifetime.
		float get_lifetime() const;
		/// Returns the starttime of the particle. See @ref starttime.
		float get_starttime() const;
		/// Returns whether the particle is fixed or not. See @ref fixed.
		bool is_fixed() const;
		/// Returns the index of the particle.
		size_t get_index() const;
};

} // -- namespace sim
