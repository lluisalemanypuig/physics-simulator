#pragma once

// simulator includes
#include <simulator/geometry/geometry.hpp>
#include <simulator/geometry/triangle.hpp>
#include <simulator/geometry/sphere.hpp>
#include <simulator/geometry/plane.hpp>

// glm includes
#include <glm/vec3.hpp>

/*
 * Simple wrapper on the geometry of the
 * simulator for easy rendering.
 *
 */

class rgeom {
	private:
	public:
		rgeom() {}
		virtual ~rgeom() {}

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
		rplane() : rgeom() {}
		~rplane() {}

		physim::geom::geometry *get_underlying() {
			return pl;
		}
};

// use the three points (they should be
// on the triangle) to render the plane.
class rtriangle : public rgeom {
	private:
	public:
		glm::vec3 p1,p2,p3;
		physim::geom::triangle *tl;
	public:
		rtriangle() : rgeom() {}
		~rtriangle() {}

		physim::geom::geometry *get_underlying() {
			return tl;
		}
};

// use the center to translate a model
// of a sphere to the right position.
class rsphere : public rgeom {
	private:
	public:
		glm::vec3 c;
		physim::geom::sphere *s;
	public:
		rsphere() : rgeom() {}
		~rsphere() {}

		physim::geom::geometry *get_underlying() {
			return s;
		}
};
