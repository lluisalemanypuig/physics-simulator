#pragma once

// C++ includes
#include <functional>

// physim includes
#include <physim/particles/base_particle.hpp>

namespace physim {
namespace emitters {

/// Shortcut for the free particle emitter_base function type.
typedef std::function<void (particles::base_particle& )> base_emit;

/**
 * @brief Class for particle initialisation.
 *
 * This class is used to initialise the attributes of a particle.
 *
 * This class has several functions as attributes, each of which
 * initialises an attribute of the particle.
 *
 * These functions are:
 * - @ref pos : function used to initialise the position of a particle.
 * - @ref vel : function used to initialise the velocity of a particle.
 * - @ref mass : function to initialise the mass of the particle.
 * this function is used to initialse the radius attribute of the particle).
 *
 * By default, all these function's behaviour is to initialise a particle
 * the same way they are initialised when constructed (see @ref base_particle::init).
 *
 * Also, they are applied in the same order as they appear listed.
 * Therefore, for example, the position attributes can be used to initialise
 * the velocity. Finally, it is guaranteed that all these functions
 * will be called after the @ref simulator has assigned an index to
 * the particle being initialised.
 *
 * The previous position of a particle is updated automatically after
 * initialising all attributes using the functions, but on the simulator.
 * Therefore, there is no need to initialise it.
 *
 * The initialisation of the particle takes place in the method
 * @ref initialise_particle(base_particle *)const.
 *
 * Finally, all classes must implement the @ref clone function.
 */
class base_emitter {
	protected:
		/// Initialiser of position.
		base_emit pos;
		/// Initialiser of velocity.
		base_emit vel;
		/// Initialiser of the mass.
		base_emit mass;

	public:
		/// Default constructor.
		base_emitter();
		/// Copy constructor.
		base_emitter(const base_emitter& i);
		/// Destructor.
		virtual ~base_emitter();

		// SETTERS

		/// Sets the position emitter_base. See @ref pos.
		void set_pos_initialiser(const base_emit& f);
		/// Sets the velocity emitter_base. See @ref vel.
		void set_vel_initialiser(const base_emit& f);
		/// Sets the mass emitter_base. See @ref mass.
		void set_mass_initialiser(const base_emit& f);

		// GETTERS

		/// Returns a reference to a copy of this emitter_base.
		virtual base_emitter *clone() const;

		/// Returns the position emitter_base. See @ref pos.
		const base_emit& get_pos_initialiser() const;
		/// Returns the velocity emitter_base. See @ref vel.
		const base_emit& get_vel_initialiser() const;
		/// Returns the mass emitter_base. See @ref mass.
		const base_emit& get_mass_initialiser() const;

		// INITIALISE A PARTICLE

		/**
		 * @brief Initialise a particle.
		 *
		 * Each of the functions for particle initialisation
		 * are called on the particle. For details on the order,
		 * see the description of this class.
		 * @param p The particle to be initialised.
		 */
		void initialise_particle(particles::base_particle& p) const;
};

} // -- namespace emitters
} // -- namespace physim
