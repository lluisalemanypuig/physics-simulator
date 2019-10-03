/*********************************************************************
 * Real-time physics simulation project
 * Copyright (C) 2018-2019 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

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
