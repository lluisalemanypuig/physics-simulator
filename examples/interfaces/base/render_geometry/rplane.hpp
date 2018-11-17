#pragma once

// C++ includes
#include <stdint.h>
#include <memory>

// base includes
#include <base/render_geometry/rendered_geometry.hpp>
#include <base/model/rendered_model.hpp>
#include <base/box.hpp>

// physim includes
#include <physim/math/vec3.hpp>
typedef physim::math::vec3 vec3;

/*
 * Simple wrapper on the geometry of the
 * simulator for easy rendering.
 *
 */

/*
 * Use the four points (they should be on the plane)
 * to render the plane.
 *
 * The points should be given in either clockwise or
 * counterclockwise order
 */
class rplane : public rgeom {
	private:
		vec3 _p1,_p2,_p3,_p4;
	public:
		rplane();
		~rplane();

		// SETTERS

		void set_points
		(const vec3& p1, const vec3& p2,
		 const vec3& p3, const vec3& p4);

		// GETTERS

		const vec3& p1() const;
		const vec3& p2() const;
		const vec3& p3() const;
		const vec3& p4() const;

		// OTHERS

		void draw_geometry() const;
		void make_box(box& b) const;
};
