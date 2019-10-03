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

#include <physim/geometry/geometry.hpp>

// C++ includes
#include <limits>
using namespace std;

// physim includes
#include <physim/math/private/math3/base.hpp>
#include <physim/math/private/math3/geometry.hpp>

namespace physim {
using namespace math;

namespace geometric {

// PUBLIC

geometry::geometry() {
	__pm3_assign_s(vmin,  numeric_limits<float>::max());
	__pm3_assign_s(vmax, -numeric_limits<float>::max());
}
geometry::geometry(const geometry& g) {
	__pm3_assign_v(vmin, g.vmin);
	__pm3_assign_v(vmax, g.vmax);
}
geometry::~geometry() { }

// GETTERS

geometry_type geometry::get_geom_type() const {
	return geometry_type::none;
}

const vec3& geometry::get_min() const {
	return vmin;
}
const vec3& geometry::get_max() const {
	return vmax;
}

vec3 geometry::get_box_center() const {
	return (vmin + vmax)/2;
}

float geometry::approx_radius() const {
	return __pm3_dist(get_box_center(), vmax);
}

} // -- namespace geometric
} // -- namespace physim
