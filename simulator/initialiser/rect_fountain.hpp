#pragma once

// C++ includes
#include <random>
using namespace std;

// Custom includes
#include <simulator/initialiser/rectangular_source.hpp>

namespace physim {
namespace init {

/**
 * @brief A fountain class initialiser.
 *
 * Provides a position initialiser function so that generated
 * particles behave like a fountain: their velocity is set to
 * positive values of y, and random values of x and z.
 *
 * Need to set the source size through
 * @ref rect_source::set_rectangle_source method
 * so that the initialser's source is defined.
 */
class rect_fountain : public rect_source {
	protected:
		/**
		 * @brief Sets the velocity initialser.
		 *
		 * It is made a virtual function so that this class can be
		 * reimplemented in another one.
		 */
		void make_vel_init();

	public:
		/// Default constructor.
		rect_fountain();
		/// Copy constructor.
		rect_fountain(const rect_fountain& f);
		/// Destructor.
		~rect_fountain();

};

} // -- namespace init
} // -- namespace physim
