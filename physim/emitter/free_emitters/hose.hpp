/*********************************************************************
 * Real-time physics simulation project
 * Copyright (C) 2018-2019 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

#pragma once

// C++ includes
#include <random>

// physim includes
#include <physim/emitter/free_emitter.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace emitters {
namespace free_emitters {

/**
 * @brief A hose emitter.
 *
 * This emitter tries to make the particles have the
 * same behaviour as the water coming out of a hose. This
 * so-called hose has been abstracted as a cone.
 *
 * The cone is parametrised with a source, the vertex at the
 * peak of the cone (see @ref source), and with a circular base,
 * centered at a point (see @ref cc), that has radius @ref r.
 * The distance between the vertex and the center of the base
 * is the height of the cone (see @ref h). For easier calculations
 * this object stores the perpendicular unit vectors @ref v and @ref w
 * that span the plane containing the base.
 *
 * In order to define this cone one has to use method
 * @ref set_hose_source. This method needs the @ref source of the
 * hose (the peak of the cone), a unit vector that points
 * from the source to the base, the radius of the base @ref r,
 * and finally the height of the cone, @ref h.
 *
 * Particles are initialised with initial position @ref source.
 * The velocity is a random vector that goes from the @ref source
 * to a randomly generated point on the base of the cone.
 * Therefore the minimum speed the particles will have is
 * - \f$h\f$ (in case the point generated is exactly on the
 * middle of the base),
 * and the maximum is
 * - \f$\sqrt{h^2 + r^2}\f$ (in case the point generated lies
 * on the circumference of the base).
 */
class hose : public free_emitter {
	protected:
		/// Engine used in the uniform distribution @ref U01.
		std::default_random_engine E;
		/// Random number generator for uniform values between 0 and 1.
		std::uniform_real_distribution<float> U01;

		/// The vertex (apex) of the cone.
		math::vec3 source;
		/// The center of the cone's base.
		math::vec3 cc;

		/**
		 * @brief Unit vector on the circle.
		 *
		 * It is perpendicular to @ref w.
		 */
		math::vec3 v;
		/**
		 * @brief Unit vector on the circle.
		 *
		 * It is perpendicular to @ref v.
		 */
		math::vec3 w;

		/// Radius of the cone.
		float r;
		/// Height of the cone.
		float h;

	protected:
		/**
		 * @brief Sets the position initialser.
		 *
		 * It is made a virtual function so that this class can be
		 * reimplemented in another one.
		 *
		 * Positions are generated according to the parametrisation
		 * of this rectangle.
		 */
		virtual void make_pos_init();
		/**
		 * @brief Sets the velocity initialser.
		 *
		 * It is made a virtual function so that this class can be
		 * reimplemented in another one.
		 */
		virtual void make_vel_init();

	public:
		/// Default constructor.
		hose();
		/**
		 * @brief Copy constructor.
		 *
		 * The functions @ref base_emitter::pos and @ref base_emitter::vel
		 * are not copied. Instead, they are remade (functions
		 * @ref make_pos_init and @ref make_vel_init are called again).
		 */
		hose(const hose& h);
		/// Destructor.
		virtual ~hose();

		// SETTERS

		/**
		 * @brief Sets the source of the hose.
		 * @param S See @ref source.
		 * @param u Unit vector from the source to the center of the
		 * base. Notice that the center of the base (@ref cc) is
		 * calculated as S + h*u.
		 * @param _r See @ref r.
		 * @param _h See @ref h.
		 */
		void set_hose_source
		(const math::vec3& S, const math::vec3& u, float _r,float _h);

		// GETTERS

		free_emitter *clone() const;

};

} // -- namespace free_emitters
} // -- namespace emitters
} // -- namespace physim
