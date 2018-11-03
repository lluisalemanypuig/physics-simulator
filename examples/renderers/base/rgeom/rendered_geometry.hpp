#pragma once

// C++ includes
#include <stdint.h>

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
		rendered_model *model;

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
		float r,g,b,a;

		rgeom();
		virtual ~rgeom();

		// SETTERS

		void set_render(bool r);
		void set_color(float _r, float _g, float _b, float _a);
		void set_model(rendered_model *model);

		// GETTERS

		bool should_render() const;
		rendered_geometry_type get_type() const;
		rendered_model *get_model();

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

/**
 * @brief Rendered plane class
 *
 * Use the four points (they should be
 * on the plane) to render the plane.
 *
 * The points should be given in either
 * clockwise or counterclockwise order
 */
class rplane : public rgeom {
	private:
	public:
		vec3 p1,p2,p3,p4;
	public:
		rplane();
		~rplane();

		// OTHERS

		void draw_geometry() const;
		void make_box(box& b) const;
};

// use the three points (they should be
// on the triangle) to render the plane.
class rtriangle : public rgeom {
	private:
	public:
		vec3 p1,p2,p3;

	public:
		rtriangle();
		~rtriangle();

		// OTHERS

		void draw_geometry() const;
		void make_box(box& b) const;
};

// use the four points (they should be
// on the triangle) to render the plane.
class rrectangle : public rgeom {
	private:
	public:
		vec3 p1,p2,p3,p4;

	public:
		rrectangle();
		~rrectangle();

		// OTHERS

		void draw_geometry() const;
		void make_box(box& b) const;
};

// use the center to translate a model
// of a sphere to the right position.
class rsphere : public rgeom {
	private:
	public:
		vec3 c;
		float r;

	public:
		rsphere();
		~rsphere();

		// OTHERS

		void translate_object() const;
		void draw_geometry() const;
		void make_box(box& b) const;
};
