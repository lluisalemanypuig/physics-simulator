#pragma once

// C++ includes
#include <stdint.h>
#include <memory>

// base includes
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

enum class rendered_geometry_type : int8_t {
	none = -1,
	plane,
	triangle,
	rectangle,
	sphere
};

class rgeom {
	protected:
		bool render;
		rendered_geometry_type t;
		std::shared_ptr<rendered_model> _model;

		// flat color of the model
		float r,g,b,a;

		/**
		 * @brief Translational operations
		 *
		 * An object may be needed to be translated,
		 * like a sphere. Therefore, this method is
		 * implemented using glTranslate and/or glRotate.
		 *
		 * Planes, triangles, and rectangles need not
		 * implement this method since they are given
		 * their coordinates and are generated using
		 * glBegin, glEnd.
		 */
		virtual void translate_object() const;

		/**
		 * @brief Draws the geometry.
		 *
		 * Using glBegin, glEnd, for example.
		 */
		virtual void draw_geometry() const = 0;

	public:
		rgeom();
		virtual ~rgeom();

		// MODIFIERS

		void clear();

		// SETTERS

		void set_render(bool r);
		void set_color(float _r, float _g, float _b, float _a);
		void set_model(const std::shared_ptr<rendered_model>& _model);

		// GETTERS

		bool should_render() const;
		rendered_geometry_type get_type() const;
		std::shared_ptr<rendered_model> get_model();

		float red() const;
		float green() const;
		float blue() const;
		float alpha() const;

		// OTHERS

		/**
		 * @brief Draws the geometry.
		 *
		 * In case this geometry has a rendered model associated
		 * (see @ref model) then its method
		 * @ref rendered_model::render() is called. Otherwise,
		 * the function calls the pure virtual method
		 * @ref draw_geometry().
		 *
		 * For the former case, an extra function is called:
		 * @ref translate_object. This function contains
		 * calls to glTranslatef and glRotatef to place the
		 * object where it should be.
		 */
		void draw() const;

		/// Makes the bounding box of this model.
		virtual void make_box(box& b) const = 0;
};





