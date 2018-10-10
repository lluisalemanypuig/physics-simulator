#pragma once

// simulator includes
#include <simulator/geometry/geometry.hpp>
#include <simulator/geometry/rectangle.hpp>
#include <simulator/geometry/triangle.hpp>
#include <simulator/geometry/sphere.hpp>
#include <simulator/geometry/plane.hpp>

// Qt includes
#include <QVector4D>

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
		QVector4D color;

	public:
		rgeom() {
			render = true;
			color = QVector4D(1.0f, 0.0f, 0.0f, 1.0f);
		}
		virtual ~rgeom() {}

		// SETTERS

		void set_render(bool r) { render = r; }
		void set_color(const QVector4D& c) { color = c; }

		// GETTERS

		bool should_render() const { return render; }
		const QVector4D& get_color() const { return color; }
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

// use the four points (they should be
// on the triangle) to render the plane.
class rrectangle : public rgeom {
	private:
	public:
		glm::vec3 p1,p2,p3,p4;
		physim::geom::rectangle *rl;
	public:
		rrectangle() : rgeom() {
			rl = nullptr;
		}
		~rrectangle() {}

		physim::geom::geometry *get_underlying() {
			return rl;
		}
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
		rsphere() : rgeom() {
			s = nullptr;
		}
		~rsphere() {}

		physim::geom::geometry *get_underlying() {
			return s;
		}
};
