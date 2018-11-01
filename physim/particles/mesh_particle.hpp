#pragma once

// C inlcudes
#include <stddef.h>

// physim includes
#include <physim/math/vec3.hpp>

namespace physim {
namespace particles {

/**
 * @brief Class implementing a particle.
 *
 * A particle is a 0-dimensional object, subject to
 * several forces. It can also collide with other
 * objects in the scene (geometrical objects, see
 * namespace @ref physim::geom) but not with other
 * particles.
 *
 * When specified in the simulator, the moving particle
 * may create a variable electric field that acts on
 * the other particles.
 */
class mesh_particle {
	private:
		/// Previous position of the particle [m].
		math::vec3 prev_pos;
		/// Current position of the particle [m].
		math::vec3 cur_pos;
		/// Current velocity of the particle [m/s].
		math::vec3 cur_velocity;
		/// Force currently applied to the particle [N].
		math::vec3 force;

		/// Mass of the particle [Kg].
		float mass;
		/// Electrical charge of the particle [C].
		float charge;

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
		 * - @ref cur_velocity : vec3(0,0,0)
		 * - @ref force : vec3(0,0,0)
		 * - @ref mass : 1
		 * - @ref charge : 0
		 * - @ref fixed : false
		 * - @ref index : no value assigned, since it will be
		 * overwritten by the simulator.
		 *
		 * The current position (@ref cur_pos) is not initialised
		 * since it will be overwritten later in the methods that
		 * also call this one.
		 */
		void init();

	public:
		/// Default constructor.
		mesh_particle();
		/// Copy constructor.
		mesh_particle(const mesh_particle& p);
		/// Destructor.
		~mesh_particle();

		// MODIFIERS

		/**
		 * @brief Translate the particle according to vector @e v.
		 *
		 * Increment current position (see @ref cur_pos) by @e v.
		 * @param v The direction of the translation.
		 */
		void translate(const math::vec3& v);
		/**
		 * @brief Increment the velocity of the particle.
		 *
		 * Increments the velocity of the particle (see @ref cur_velocity)
		 * by @e v.
		 * @param v The direction of the acceleration.
		 */
		void accelerate(const math::vec3& v);

		/**
		 * @brief Adds @e (x,y,z) to the particle's @ref force.
		 * @post @ref force += @e (x,y,z)
		 */
		void add_force(const float& x, const float& y, const float& z);
		/**
		 * @brief Adds @e f to the particle's @ref force.
		 * @post @ref force += @e f
		 */
		void add_force(const math::vec3& f);

		/**
		 * @brief Saves the current position in the particle's state.
		 *
		 * Copies @ref cur_pos into @ref prev_pos.
		 */
		void save_position();

		// SETTERS

		/// Sets the previous position of the particle. See @ref prev_pos.
		void set_previous_position(const float& x, const float& y, const float& z);
		/// Sets the previous position of the particle. See @ref prev_pos.
		void set_previous_position(const math::vec3& pos);

		/// Sets the current position of the particle. See @ref cur_pos.
		void set_position(const float& x, const float& y, const float& z);
		/// Sets the current position of the particle. See @ref cur_pos.
		void set_position(const math::vec3& pos);

		/// Sets the current velocity of the particle. See @ref cur_velocity.
		void set_velocity(const float& x, const float& y, const float& z);
		/// Sets the current velocity of the particle. See @ref cur_velocity.
		void set_velocity(const math::vec3& vel);

		/// Sets the force of the particle. See @ref force.
		void set_force(const float& x, const float& y, const float& z);
		/// Sets the force of the particle. See @ref force.
		void set_force(const math::vec3& f);

		/// Sets the mass of the particle. See @ref mass.
		void set_mass(float m);
		/// Sets the charge of this particle. See @ref charge.
		void set_charge(float c);
		/// Sets whether this particle is fixed or not. See @ref fixed.
		void set_fixed(bool f);

		/// Sets the index of the particle. See @ref index.
		void set_index(size_t i);

		// GETTERS

		/// Returns the previous position. See @ref prev_pos.
		math::vec3& get_previous_position();
		/// Returns the previous position. See @ref prev_pos.
		const math::vec3& get_previous_position() const;
		/// Returns the current position. See @ref cur_pos.
		math::vec3& get_position();
		/// Returns the current position. See @ref cur_pos.
		const math::vec3& get_position() const;
		/// Returns the current velocity of the particle. See @ref cur_velocity.
		math::vec3& get_velocity();
		/// Returns the current velocity of the particle. See @ref cur_velocity.
		const math::vec3& get_velocity() const;
		/// Returns the force applied to the particle. See @ref force.
		math::vec3& get_force();
		/// Returns the force applied to the particle. See @ref force.
		const math::vec3& get_force() const;
		/// Returns the mass of the particle. See @ref mass.
		float get_mass() const;
		/// Returns the charge of the particle. See @ref charge.
		float get_charge() const;
		/// Returns whether the particle is fixed or not. See @ref fixed.
		bool is_fixed() const;
		/// Returns the index of the particle.
		size_t get_index() const;
};

} // -- namespace particles
} // -- namespace sim
