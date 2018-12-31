#pragma once

// C++ includes
#include <functional>

// physim includes
#include <physim/emitter/free_emitter.hpp>
#include <physim/particles/sized_particle.hpp>

namespace physim {
namespace emitters {

/// Shortcut for the sized particle emitter_sized function type.
typedef std::function<void (particles::sized_particle& )> sized_emit;

/**
 * @brief Class for sized particle initialisation.
 *
 * This class is used to initialise the attributes of a sized particle.
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
 * - @ref radius : for the case of sized particles (see @ref particles::sized_particle,
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
 * @ref initialise_particle(sized_particle *)const.
 *
 * Finally, all classes must implement the @ref clone function.
 */
class sized_emitter : public free_emitter {
	protected:
		/// Initialiser of the 'R' attribute.
		sized_emit radius;

	public:
		/// Default constructor.
		sized_emitter();
		/// Copy constructor.
		sized_emitter(const sized_emitter& i);
		/// Destructor.
		virtual ~sized_emitter();

		// SETTERS

		/// Sets the radius emitter_sized. See @ref radius.
		void set_radius_initialiser(const sized_emit& f);

		// GETTERS

		/// Returns a reference to a copy of this emitter_sized.
		virtual sized_emitter *clone() const;

		/// Returns the radius emitter_sized. See @ref radius.
		const sized_emit& get_radius_initialiser() const;

		// INITIALISE A PARTICLE

		/**
		 * @brief Initialise a particle.
		 *
		 * Each of the functions for particle initialisation
		 * are called on the particle. For details on the order,
		 * see the description of this class.
		 * @param p The particle to be initialised.
		 */
		void initialise_particle(particles::sized_particle& p) const;
};

} // -- namespace emitters
} // -- namespace physim
