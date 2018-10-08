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
	protected:
		bool render;
	public:
		rgeom() {
			render = true;
		}
		virtual ~rgeom() {}

		// SETTERS

		void set_render(bool r) { render = r; }

		// GETTERS

		bool should_render() const { return render; }
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
		rplane() : rgeom() {
			pl = nullptr;
		}
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
		rtriangle() : rgeom() {
			tl = nullptr;
		}
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
		rsphere() : rgeom() {
			s = nullptr;
		}
		~rsphere() {}

		physim::geom::geometry *get_underlying() {
			return s;
		}
};
