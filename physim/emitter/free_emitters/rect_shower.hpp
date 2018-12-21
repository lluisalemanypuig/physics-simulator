#pragma once

// C++ includes
#include <random>

// physim includes
#include <physim/emitter/free_emitters/rect_source.hpp>

namespace physim {
namespace emitters {
namespace free_emitters {

/**
 * @brief A shower class emitter_free.
 *
 * Provides a position emitter_free function so that generated
 * particles behave like a shower: their velocity is set to
 * negative values of y, and null values of x and z.
 *
 * Need to set the source size through
 * @ref rect_source::set_rectangular_source method
 * so that the initialser's source is defined.
 */
class rect_shower : public rect_source {
	protected:
		/**
		 * @brief Sets the velocity initialser.
		 *
		 * The velocity is set to negative values of y, and
		 * null values of x and z.
		 */
		void make_vel_init();

	public:
		/// Default constructor.
		rect_shower();
		/// Copy constructor.
		rect_shower(const rect_shower& w);
		/// Destructor.
		~rect_shower();

		// GETTERS

		free_emitter *clone() const;

};

} // -- namespace free_emitters
} // -- namespace emitters
} // -- namespace physim
