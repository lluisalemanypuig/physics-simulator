#pragma once

// C++ includes
#include <random>
using namespace std;

// Custom includes
#include <simulator/initialiser/rectangular_source.hpp>

namespace physim {
namespace init {

/**
 * @brief A waterfall class initialiser.
 *
 * Provides a position initialiser function so that generated
 * particles behave like a waterfall: their velocity is set to
 * negative values of y, and null values of x and z.
 *
 * Need to set the source size through
 * @ref rectangular_source::set_rectangle_source method
 * so that the initialser's source is defined.
 */
class rect_waterfall : public rectangular_source {
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
		rect_waterfall();
		/// Destructor.
		~rect_waterfall();

};

} // -- namespace init
} // -- namespace physim
