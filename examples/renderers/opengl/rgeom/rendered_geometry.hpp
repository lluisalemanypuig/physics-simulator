#pragma once

// physim includes
#include <physim/geometry/geometry.hpp>
#include <physim/geometry/rectangle.hpp>
#include <physim/geometry/triangle.hpp>
#include <physim/geometry/sphere.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/math/vec3.hpp>

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

	public:
		float r,g,b,a;

		rgeom();
		virtual ~rgeom();

		// SETTERS

		void set_render(bool r);
		void set_color(float _r, float _g, float _b, float _a);

		// GETTERS

		bool should_render() const;
		rendered_geometry_type get_type() const;
};

// use the four points (they should be
// on the plane) to render the plane.
class rplane : public rgeom {
	private:
	public:
		physim::math::vec3 p1,p2,p3,p4;
	public:
		rplane();
		~rplane();
};

// use the three points (they should be
// on the triangle) to render the plane.
class rtriangle : public rgeom {
	private:
	public:
		physim::math::vec3 p1,p2,p3;
	public:
		rtriangle();
		~rtriangle();
};

// use the four points (they should be
// on the triangle) to render the plane.
class rrectangle : public rgeom {
	private:
	public:
		physim::math::vec3 p1,p2,p3,p4;
	public:
		rrectangle();
		~rrectangle();
};

// use the center to translate a model
// of a sphere to the right position.
class rsphere : public rgeom {
	private:
	public:
		physim::math::vec3 c;
		float r;
	public:
		rsphere();
		~rsphere();
};
