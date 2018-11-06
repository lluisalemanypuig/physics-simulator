#pragma once

// C++ includes
#include <vector>

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/initialiser/initialiser.hpp>

namespace physim {
namespace init {

/**
 * @brief Multiple source initialiser.
 *
 * This class implements an initialiser that uses multiple
 * initialisers of the same type.
 *
 * This can be used, for example, to have several hoses in
 * the same simulator.
 *
 * In order to achieve this, the class divides into chunks
 * of similar size all the particles in the simulation.
 * If there are \f$n\f$ particles in the simulation and
 * this class has \f$s\f$ sources then every chunk will
 * have \f$n/s\f$ particles in it. The @e i-th chunk is
 * assigned the particles with index
 *
 * \f$[i*\lfloor n/s \rfloor), (i + 1)*\lfloor n/s \rfloor)\f$,
 * \f$[(s - 1)*\lfloor n/s \rfloor), n\f$
 *
 * for \f$i \in [0,s - 2] \f$
 *
 * Since $\f$n\f$ may not be a multiple of \f$s\f$ then the
 * last chunk will have a few more particles than the others.
 *
 * In particular, the last chunk will have
 * \f$n - (s - 1)*\lfloor n/s \rfloor) + 1\f$
 * particles.
 *
 * The sources should be set by retrieving the reference
 * to the sources container (see @ref get_sources) after
 * allocating the wanted amount (see @ref allocate).
 *
 * Some of the functions used to initialise the chunks
 * of particles will have to be remade in the copy constructor.
 * The functions to be remade are indicated by calling the
 * use_*_init functions. Notice that when using the @ref hose
 * initialiser the position and velocity init must be remade.
 */
template<class T>
class multisource : public initialiser {
	private:
		/// Number of particles in the simulation.
		size_t P;
		/// Collection of sources for this multiple source initialiser.
		std::vector<T> sources;

		/**
		 * @brief Bit mask that indicates what functions are used.
		 *
		 * When copy-constructing an object of this type, some
		 * functions will have to be remade (using the appropriate
		 * make_*_init functions).
		 *
		 * The least-weight bits of this integer represent
		 * the 9 functions available. If any of these 9 bits
		 * is set to one, then the corresponding function will
		 * be made.
		 */
		unsigned int use_functions;

	public:
		/// Default constructor.
		multisource();
		/// Destructor.
		~multisource();

		/**
		 * @brief Allocates space for @e n initialisers.
		 *
		 * Resizes @ref sources to have @e n elements.
		 * @param N Number of particles in the simulation.
		 * @param n Number of initialisers.
		 */
		void allocate(size_t N, size_t n);

		// MODIFIERS

		/**
		 * @brief The position initialiser will be remade when copying.
		 *
		 * Sets or unsets the 0 bit of @ref use_functions;
		 * @param u Either true or false.
		 */
		void use_position_init(bool u = true);
		/**
		 * @brief The velocity initialiser will be remade when copying.
		 *
		 * Sets or unsets the 1 bit of @ref use_functions;
		 * @param u Either true or false.
		 */
		void use_velocity_init(bool u = true);
		/**
		 * @brief The mass initialiser will be remade when copying.
		 *
		 * Sets or unsets the 2 bit of @ref use_functions;
		 * @param u Either true or false.
		 */
		void use_mass_init(bool u = true);
		/**
		 * @brief The charge initialiser will be remade when copying.
		 *
		 * Sets or unsets the 3 bit of @ref use_functions;
		 * @param u Either true or false.
		 */
		void use_charge_init(bool u = true);
		/**
		 * @brief The bouncing coefficient initialiser will be remade when copying.
		 *
		 * Sets or unsets the 4 bit of @ref use_functions;
		 * @param u Either true or false.
		 */
		void use_bounce_init(bool u = true);
		/**
		 * @brief The friction coefficient initialiser will be remade when copying.
		 *
		 * Sets or unsets the 5 bit of @ref use_functions;
		 * @param u Either true or false.
		 */
		void use_friction_init(bool u = true);
		/**
		 * @brief The lifetime initialiser will be remade when copying.
		 *
		 * Sets or unsets the 6 bit of @ref use_functions;
		 * @param u Either true or false.
		 */
		void use_lifetime_init(bool u = true);
		/**
		 * @brief The starttime initialiser will be remade when copying.
		 *
		 * Sets or unsets the 7 bit of @ref use_functions;
		 * @param u Either true or false.
		 */
		void use_starttime_init(bool u = true);
		/**
		 * @brief The fixed attribute initialiser will be remade when copying.
		 *
		 * Sets or unsets the 8 bit of @ref use_functions;
		 * @param u Either true or false.
		 */
		void use_fixed_init(bool u = true);

		/**
		 * @brief Make position initialiser. See @ref initialiser::pos.
		 *
		 * The @e i-th particle is assigned its position using
		 * the @e k-th source. See description of the class for details
		 * on what source is used.
		 */
		void make_position_init();
		/**
		 * @brief Make velocity initialiser. See @ref initialiser::vel.
		 *
		 * The @e i-th particle is assigned its velocity using
		 * the @e k-th source. See description of the class for details
		 * on what source is used.
		 */
		void make_velocity_init();
		/**
		 * @brief Make mass initialiser. See @ref initialiser::mass.
		 *
		 * The @e i-th particle is assigned its mass using
		 * the @e k-th source. See description of the class for details
		 * on what source is used.
		 */
		void make_mass_init();
		/**
		 * @brief Make charge initialiser. See @ref initialiser::charge.
		 *
		 * The @e i-th particle is assigned its charge using
		 * the @e k-th source. See description of the class for details
		 * on what source is used.
		 */
		void make_charge_init();
		/**
		 * @brief Make bounce initialiser. See @ref initialiser::bounce.
		 *
		 * The @e i-th particle is assigned its bouncing coefficient using
		 * the @e k-th source. See description of the class for details
		 * on what source is used.
		 */
		void make_bounce_init();
		/**
		 * @brief Make friction initialiser. See @ref initialiser::friction.
		 *
		 * The @e i-th particle is assigned its position using
		 * the @e k-th source. See description of the class for details
		 * on what source is used.
		 */
		void make_friction_init();
		/**
		 * @brief Make lifetime initialiser. See @ref initialiser::lifetime.
		 *
		 * The @e i-th particle is assigned its lifetime using
		 * the @e k-th source. See description of the class for details
		 * on what source is used.
		 */
		void make_lifetime_init();
		/**
		 * @brief Make starttime initialiser. See @ref initialiser::starttime.
		 *
		 * The @e i-th particle is assigned its starttime using
		 * the @e k-th source. See description of the class for details
		 * on what source is used.
		 */
		void make_starttime_init();
		/**
		 * @brief Make fixed initialiser. See @ref initialiser::fixed.
		 *
		 * The @e i-th particle is assigned its 'fixed' attribute using
		 * the @e k-th source. See description of the class for details
		 * on what source is used.
		 */
		void make_fixed_init();

		/// Calls all make_*_init functions.
		void make_all_init();

		// GETTERS

		/// Returns the amount of sources of this initialiser.
		size_t size() const;

		/// Returns a reference to the sources.
		std::vector<T>& get_sources();
		/// Returns a constant reference to the sources.
		const std::vector<T>& get_sources() const;

		// OTHERS

		initialiser *clone() const;
};

} // -- namespace init
} // -- namespace physim

#include <physim/initialiser/multisource.cpp>
