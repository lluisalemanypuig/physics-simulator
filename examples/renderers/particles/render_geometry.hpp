#pragma once

// Qt includes
#include <QVector4D>

// physim includes
#include <physim/geometry/geometry.hpp>
#include <physim/geometry/rectangle.hpp>
#include <physim/geometry/triangle.hpp>
#include <physim/geometry/sphere.hpp>
#include <physim/geometry/plane.hpp>

// glm includes
#include <glm/vec3.hpp>
using namespace glm;

/*
 * Simple wrapper on the geometry of the
 * simulator for easy rendering.
 *
 */

class rgeom {
	protected:
		bool render;
		vec4 color;

	public:
		rgeom();
		virtual ~rgeom();

		// SETTERS

		void set_render(bool r);
		void set_color(const vec4& c);

		// GETTERS

		bool should_render() const;
		const vec4& get_color() const;
		virtual physim::geom::geometry *get_underlying() = 0;
};

// use the four points (they should be
// on the plane) to render the plane.
class rplane : public rgeom {
	private:
	public:
		glm::vec3 p1,p2,p3,p4;
		physim::geom::plane *pl;
	public:
		rplane();
		~rplane();
		physim::geom::geometry *get_underlying();
};

// use the three points (they should be
// on the triangle) to render the plane.
class rtriangle : public rgeom {
	private:
	public:
		glm::vec3 p1,p2,p3;
		physim::geom::triangle *tl;
	public:
		rtriangle();
		~rtriangle();

		physim::geom::geometry *get_underlying();
};

// use the four points (they should be
// on the triangle) to render the plane.
class rrectangle : public rgeom {
	private:
	public:
		glm::vec3 p1,p2,p3,p4;
		physim::geom::rectangle *rl;
	public:
		rrectangle();
		~rrectangle();

		physim::geom::geometry *get_underlying();
};

// use the center to translate a model
// of a sphere to the right position.
class rsphere : public rgeom {
	private:
	public:
		glm::vec3 c;
		float r;
		physim::geom::sphere *s;
	public:
		rsphere();
		~rsphere();

		physim::geom::geometry *get_underlying();
};
