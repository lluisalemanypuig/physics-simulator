#pragma once

// glm includes
#include <glm/vec3.hpp>

// physim includes
#include <physim/math/vec3.hpp>

// render includes
#include <render/geometry/rgeometry.hpp>
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/box.hpp>

/*
 * Simple wrapper on the geometry of the
 * simulator for easy rendering.
 *
 */

typedef std::vector<std::pair<physim::math::vec3, physim::math::vec3> >
	physim_boxes_set;

// use the center to translate a mesh
// of a sphere to the right position.
class robject : public rgeom {
	private:
		 std::vector<box> boxes;

	public:
		robject();
		~robject();

		// SETTERS

		void set_boxes(const physim_boxes_set& pbs);

		// GETTERS

		const std::vector<box>& get_boxes() const;

		// OTHERS

		void make_boxes_buffers();

		void draw_geometry() const;
		void make_model_matrix(glm::mat4& mat) const;
		void make_box(box& b) const;
};
