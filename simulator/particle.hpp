#pragma once

// glm includes
#include <glm/glm.hpp>
using namespace glm;

namespace sim {

/**
 * @brief Class implementing a particle.
 *
 * A particle is a 0-dimensional object, subject to
 * several forces. It can also collide with other
 * objects in the scene (geometrical objects, see
 * namespace @ref sim::geom) but not with other
 * particles.
 */
class particle {
	private:
		/// Current position of the particle.
		vec3 cur_pos;
		/// Previous position of the particle.
		vec3 prev_pos;
		/// Force currently applied to the particle.
		vec3 force;
		// Current velocity of the particle.
		vec3 velocity;

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
		 * @brief Is this particle fixed?
		 *
		 * If the particle, it will be ignored by the solver, therefore
		 * not taken into account in the simulation (gravity nor any
		 * other force will have any effect on it).
		 */
		bool fixed;

	private:

		/// Initialises all particle's attribute to null values
		/// except for @ref cur_pos.
		void init();

	public:
		/// Default constructor.
		particle();
		/// Construct particle with position as 3 float values.
		particle(const float& x, const float& y, const float& z);
		/// Construct particle with position as a vec3.
		particle(const vec3& pos);
		/// Destructor.
		~particle();

		// MODIFIERS

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
		 * @brief Translate the particle according to vector @e v.
		 *
		 * Increment current position (see @ref cur_pos) by @e v.
		 * @param v The direction of the translation.
		 */
		void translate(const vec3& v);
		/**
		 * @brief Increment the velocity of the particle.
		 *
		 * Increments the velocity of the particle (see @ref velocity)
		 * by @e v.
		 * @param v The direction of the acceleration.
		 */
		void acceleterate(const vec3& v);

		// SETTERS

		/// Sets the current position of the particle. See @ref cur_pos.
		void set_position(const float& x, const float& y, const float& z);
		/// Sets the current position of the particle. See @ref cur_pos.
		void set_position(const vec3& pos);

		/// Sets the previous position of the particle. See @ref prev_pos.
		void set_prev_position(const float& x, const float& y, const float& z);
		/// Sets the previous position of the particle. See @ref prev_pos.
		void set_prev_position(const vec3& pos);

		/// Sets the velocity of the particle. See @ref velocity.
		void set_velocity(const float& x, const float& y, const float& z);
		/// Sets the velocity of the particle. See @ref velocity.
		void set_velocity(const vec3& vel);

		/// Sets the force of the particle. See @ref force.
		void set_force(const float& x, const float& y, const float& z);
		/// Sets the force of the particle. See @ref force.
		void set_force(const vec3& f);

		/// Sets the bouncing coefficient of the particle. See @ref bouncing.
		void set_bouncing(float b);
		/// Sets the lifetime of the particle. See @ref lifetime.
		void set_lifetime(float lT);
		/// Sets whether this particle is fixed or not. See @ref fixed.
		void set_fixed(bool f);

		// GETTERS

		/// Returns the current position. See @ref cur_pos.
		vec3 get_current_position() const;
		/// Returns the previous position. See @ref prev_pos.
		vec3 get_previous_position() const;
		/// Returns the force applied to the particle. See @ref force.
		vec3 get_force() const;
		/// Returns the velocity of the particle. See @ref velocity.
		vec3 get_velocity() const;
		/// Returns the bouncing coefficient of the particle. See @ref bouncing.
		float get_bouncing() const;
		/// Returns the lifetime of the particle. See @ref lifetime.
		float get_lifetime() const;
		/// Returns whether the particle is fixed or not. See @ref fixed.
		bool is_fixed() const;
};

} // -- namespace sim
