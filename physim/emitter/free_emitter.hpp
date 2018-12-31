#pragma once

// C++ includes
#include <functional>

// physim includes
#include <physim/emitter/base_emitter.hpp>
#include <physim/particles/free_particle.hpp>

namespace physim {
namespace emitters {

/// Shortcut for the free particle initialiser function type.
typedef std::function<void (particles::free_particle& )> free_emit;

/**
 * @brief Class for free particle initialisation.
 *
 * This class is used to initialise the attributes of a free particle.
 *
 * This class has several functions as attributes, each of which
 * initialises an attribute of the particle.
 *
 * These functions are:
 * - @ref pos : function used to initialise the position of a particle.
 * - @ref vel : function used to initialise the velocity of a particle.
 * - @ref mass : function to initialise the mass of the particle.
 * - @ref charge : function used to initialise the charge of a particle.
 * - @ref friction : function used to initialise the friction coefficient of a particle.
 * - @ref bounce : function used to initialise the bouncing coefficient of a particle.
 * - @ref lifetime : function used to initialise the lifetime of a particle.
 * - @ref starttime : function used to initialise the starting time of a particle.
 * - @ref fixed : function used to initialise the fixed attribute of a particle.
 * this function is used to initialse the radius attribute of the particle).
 *
 * By default, all these function's behaviour is to initialise a particle
 * the same way they are initialised when constructed (see @ref free_particle::init).
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
 * @ref initialise_particle(free_particle *)const.
 *
 * Finally, all classes must implement the @ref clone function.
 */
class free_emitter : public base_emitter {
	protected:
		/// Initialiser of the charge.
		free_emit charge;
		/// Initialiser of the friction coefficient.
		free_emit friction;
		/// Initialiser of the bouncing coefficient.
		free_emit bounce;
		/// Initialiser of the lifetime.
		free_emit lifetime;
		/// Initialiser of the starttime.
		free_emit starttime;
		/// Initialiser of the 'fixed' attribute.
		free_emit fixed;

	public:
		/// Default constructor.
		free_emitter();
		/// Copy constructor.
		free_emitter(const free_emitter& i);
		/// Destructor.
		virtual ~free_emitter();

		// SETTERS

		/// Sets the charge initialiser. See @ref charge.
		void set_charge_initialiser(const free_emit& f);
		/// Sets the bouncing coefficient initialiser. See @ref bounce.
		void set_bounce_initialiser(const free_emit& f);
		/// Sets the friction coefficient initialiser. See @ref friction.
		void set_friction_initialiser(const free_emit& f);
		/// Sets the lifetime initialiser. See @ref lifetime.
		void set_lifetime_initialiser(const free_emit& f);
		/// Sets the starttime initialiser. See @ref starttime.
		void set_starttime_initialiser(const free_emit& f);
		/// Sets the fixed initialiser. See @ref fixed.
		void set_fixed_initialiser(const free_emit& f);

		// GETTERS

		/// Returns a reference to a copy of this emitter.
		virtual free_emitter *clone() const;

		/// Returns the charge initialiser. See @ref charge.
		const free_emit& get_charge_initialiser() const;
		/// Returns the bouncing coefficient initialiser. See @ref bounce.
		const free_emit& get_bounce_initialiser() const;
		/// Returns the friction coefficient initialiser. See @ref friction.
		const free_emit& get_friction_initialiser() const;
		/// Returns the lifetime initialiser. See @ref lifetime.
		const free_emit& get_lifetime_initialiser() const;
		/// Returns the starttime initialiser. See @ref starttime.
		const free_emit& get_starttime_initialiser() const;
		/// Returns the fixed initialiser. See @ref fixed.
		const free_emit& get_fixed_initialiser() const;

		// INITIALISE A PARTICLE

		/**
		 * @brief Initialise a particle.
		 *
		 * Each of the functions for particle initialisation
		 * are called on the particle. For details on the order,
		 * see the description of this class.
		 * @param p The particle to be initialised.
		 */
		void initialise_particle(particles::free_particle& p) const;
};

} // -- namespace emitterss
} // -- namespace physim

