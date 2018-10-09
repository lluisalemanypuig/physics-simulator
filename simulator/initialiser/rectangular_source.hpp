#pragma once

// C++ includes
#include <random>
using namespace std;

// Custom includes
#include <simulator/initialiser/initialiser.hpp>

namespace physim {
namespace init {

/**
 * @brief A rectangular source class initialiser.
 *
 * Provides a position initialiser function so that they
 * are generated within a rectangle of specific size.
 *
 * Need to set the source size through @ref set_rectangle_source
 * so that the initialser's source is defined. In this method
 * is specified the coordinates of one of its corners and the
 * width and length of the rectangle.
 *
 * The points in this rectangle, then, can be found within:
 *
 * [@ref x, @ref x + @ref w] x [@ref y] x [@ref z, @ref z + @ref h]
 *
 * where 'x' denotes the cartesian product.
 *
 * The objects that are instances of its subclasses must live
 * in memory if they are to be used by the @ref simulator.
 * That is, these must be created using 'new' and be passed
 * as a pointer to the @ref simulator class using
 * @ref simulator::set_initialiser.
 */
class rect_source : public initialiser {
	protected:
		/// Engine used in the uniform distribution @ref U.
		default_random_engine E;
		/// Random number generator for uniform values between 0 and 1.
		uniform_real_distribution<float> U01;

		/// As seen from above, x-coordinate of bottom-left corner.
		float x;
		/// As seen from above, y-coordinate of bottom-left corner.
		float y;
		/// In the scene, height of the rectangle.
		float z;

		/// As seen from above, width of the rectangle.
		float w;
		/// As seen from above, height of the rectangle.
		float h;

	protected:
		/**
		 * @brief Sets the position initialser.
		 *
		 * It is made a virtual function so that this class can be
		 * reimplemented in another one.
		 *
		 * Positions are generated within the ranges [@ref x, @ref x + @ref w],
		 * and [@ref z, @ref z + @ref h].
		 */
		virtual void make_pos_init();
		/**
		 * @brief Sets the velocity initialser.
		 *
		 * It is made a virtual function so that this class can be
		 * reimplemented in another one.
		 */
		virtual void make_vel_init() = 0;

	public:
		/// Default constructor.
		rect_source();
		/**
		 * @brief Copy constructor.
		 *
		 * The function @ref initialiser::pos is not copied.
		 * Instead, it is remade (function @ref make_pos_init is
		 * called again).
		 */
		rect_source(const rect_source& rs);
		/// Destructor.
		virtual ~rect_source();

		// SETTERS

		/**
		 * @brief Sets the source of the fountain.
		 *
		 * Initialises the position function (@ref initialiser::pos),
		 * the velocity function (@ref initialiser::vel), and the
		 * starttime function (@ref initialiser::starttime).
		 * @param x As seen from above, x-coordinate of bottom-left corner.
		 * @param y As seen from above, y-coordinate of bottom-left corner.
		 * @param z In the scene, height of the rectangle.
		 * @param w As seen from above, width of the rectangle.
		 * @param h As seen from above, height of the rectangle.
		 */
		void set_rectangle_source(float x,float y, float z, float w,float h);

		// GETTERS

		virtual initialiser *clone() const = 0;

};

} // -- namespace init
} // -- namespace physim
