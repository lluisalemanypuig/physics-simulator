#pragma once

// glm includes
#include <glm/vec3.hpp>

// base includes
#include <base/geometry/rgeometry.hpp>
#include <base/model/rendered_model.hpp>
#include <base/box.hpp>

/*
 * Simple wrapper on the geometry of the
 * simulator for easy rendering.
 *
 */

// use the four points (they should be
// on the triangle) to render the plane.
class rrectangle : public rgeom {
	private:
		glm::vec3 _p1,_p2,_p3,_p4;

	public:
		rrectangle();
		~rrectangle();

		// SETTERS

		void set_points
		(const glm::vec3& p1, const glm::vec3& p2,
		 const glm::vec3& p3, const glm::vec3& p4);

		// GETTERS

		const glm::vec3& p1() const;
		const glm::vec3& p2() const;
		const glm::vec3& p3() const;
		const glm::vec3& p4() const;

		// OTHERS

		void draw_geometry() const;
		void make_box(box& b) const;
};
