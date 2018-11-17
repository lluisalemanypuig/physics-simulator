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

// use the center to translate a model
// of a sphere to the right position.
class rsphere : public rgeom {
	private:
		vec3 C;
		float R;

	public:
		rsphere();
		~rsphere();

		// SETTERS

		void set_center(const vec3& _c);
		void set_radius(float _r);

		// GETETRS

		const vec3& center() const;
		float radius() const;

		// OTHERS

		void translate_object() const;
		void draw_geometry() const;
		void make_box(box& b) const;
};
