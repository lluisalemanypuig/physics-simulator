#pragma once

// C++ includes
#include <functional>
using namespace std;

// Custom includes
#include <simulator/particle.hpp>

namespace physim {
namespace init {

/// Shortcut for the initialiser function type.
typedef function<void (particle *)> partinit;

/**
 * @brief The particle initialiser class.
 *
 * This class is used to initialise the attributes of a particle.
 *
 * This class has several functions as attributes, each of which
 * initialises an attribute of the particle.
 *
 * These functions are:
 * - @ref pos : function used to initialise the position of a particle.
 * - @ref vel : function used to initialise the velocity of a particle.
 * - @ref force : function used to initialise the force of a particle.
 * - @ref mass : function to initialise the mass of the particle.
 * - @ref friction : function used to initialise the friction coefficient of a particle.
 * - @ref bounce : function used to initialise the bouncing coefficient of a particle.
 * - @ref lifetime : function used to initialise the lifetime of a particle.
 * - @ref starttime : function used to initialise the starting time of a particle.
 * - @ref fixed : function used to initialise the fixed attribute of a particle.
 *
 * By default, all these function's behaviour is to do nothing.
 *
 * Also, they are applied in the same order as they appear listed.
 * Therefore, for example, the position attributes can be used to initialise
 * the velocity. Finally, it is guaranteed that all these functions
 * will be called after the @ref simulator has assigned an index to
 * the particle being initialised.
 *
 * The initialisation of the particle takes place in the method
 * @ref initialise_particle(particle *)const .
 */
class initialiser {
	protected:
		/// Initialiser of position.
		partinit pos;
		/// Initialiser of velocity.
		partinit vel;
		/// Initialiser of force.
		partinit force;
		/// Initialiser of the mass.
		partinit mass;
		/// Initialiser of the friction coefficient.
		partinit friction;
		/// Initialiser of the bouncing coefficient.
		partinit bounce;
		/// Initialiser of the lifetime.
		partinit lifetime;
		/// Initialiser of the starttime.
		partinit starttime;
		/// Initialiser of the 'fixed' attribute.
		partinit fixed;

	public:
		/// Default constructor.
		initialiser();
		/// Copy constructor.
		initialiser(const initialiser& i);
		/// Destructor.
		virtual ~initialiser();

		// SETTERS

		/// Sets the position initialiser. See @ref pos.
		void set_pos_initialiser(const partinit& f);
		/// Sets the velocity initialiser. See @ref vel.
		void set_vel_initialiser(const partinit& f);
		/// Sets the force initialiser. See @ref force.
		void set_force_initialiser(const partinit& f);
		/// Sets the mass initialiser. See @ref mass.
		void set_mass_initialiser(const partinit& f);
		/// Sets the bouncing coefficient initialiser. See @ref bounce.
		void set_bounce_initialiser(const partinit& f);
		/// Sets the friction coefficient initialiser. See @ref friction.
		void set_friction_initialiser(const partinit& f);
		/// Sets the lifetime initialiser. See @ref lifetime.
		void set_lifetime_initialiser(const partinit& f);
		/// Sets the starttime initialiser. See @ref starttime.
		void set_starttime_initialiser(const partinit& f);
		/// Sets the fixed initialiser. See @ref fixed.
		void set_fixed_initialiser(const partinit& f);

		// GETTERS

		/// Returns a reference to a copy of this initialiser.
		virtual initialiser *clone() const;

		/// Returns the position initialiser. See @ref pos.
		const partinit& get_pos_initialiser() const;
		/// Returns the velocity initialiser. See @ref vel.
		const partinit& get_vel_initialiser() const;
		/// Returns the force initialiser. See @ref force.
		const partinit& get_force_initialiser() const;
		/// Returns the mass initialiser. See @ref mass.
		const partinit& get_mass_initialiser() const;
		/// Returns the bouncing coefficient initialiser. See @ref bounce.
		const partinit& get_bounce_initialiser() const;
		/// Returns the friction coefficient initialiser. See @ref friction.
		const partinit& get_friction_initialiser() const;
		/// Returns the lifetime initialiser. See @ref lifetime.
		const partinit& get_lifetime_initialiser() const;
		/// Returns the starttime initialiser. See @ref starttime.
		const partinit& get_starttime_initialiser() const;
		/// Returns the fixed initialiser. See @ref fixed.
		const partinit& get_fixed_initialiser() const;

		// INITIALISE A PARTICLE

		/**
		 * @brief Initialise a particle.
		 *
		 * Each of the functions @ref pos, @ref vel, @ref force,
		 * @ref bounce, @ref friction, @ref lifetime, @ref fixed.
		 * @param p The particle to be initialised.
		 */
		void initialise_particle(particle *p) const;
};

} // -- namespace init
} // -- namespace physim
