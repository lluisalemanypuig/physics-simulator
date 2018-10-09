#pragma once

// C++ includes
#include <random>
using namespace std;

// Custom includes
#include <simulator/initialiser/rect_source.hpp>

namespace physim {
namespace init {

/**
 * @brief A fountain class initialiser.
 *
 * Provides a position initialiser function so that generated
 * particles behave like a fountain: their velocity is set to
 * positive values of the normal to the plane, and values of x
 * and z set as a function of their distance to the center plus
 * some random value.
 *
 * Need to set the source size through
 * @ref rect_source::set_rectangular_source method
 * so that the initialser's source is defined.
 */
class rect_fountain : public rect_source {
	protected:
		/**
		 * @brief Sets the velocity initialser.
		 *
		 * The velocity is set so that those points that are:
		 * - farther from the rectangle's center have a smaller-magnitude,
		 * "flatter" vector velocity, i.e., the angle between the
		 * vector and the plane is small.
		 * - closer to the center have larger-magnitude and
		 * "more vertical" vector velocity, i.e., the angle between
		 * the vector and the plane is large.
		 */
		void make_vel_init();

	public:
		/// Default constructor.
		rect_fountain();
		/**
		 * @brief Copy constructor.
		 *
		 * The function @ref initialiser::pos is not copied.
		 * Instead, it is remade (function @ref make_vel_init is
		 * called again).
		 */
		rect_fountain(const rect_fountain& f);
		/// Destructor.
		~rect_fountain();

};

} // -- namespace init
} // -- namespace physim
