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

static inline
glm::vec2 to_glm(const physim::math::vec2& v) {
	return glm::vec2(v.x,v.y);
}
static inline
glm::vec3 to_glm(const physim::math::vec3& v) {
	return glm::vec3(v.x,v.y,v.z);
}

static inline
physim::math::vec2 to_physim(const glm::vec2& v) {
	return physim::math::vec2(v.x,v.y);
}
static inline
physim::math::vec3 to_physim(const glm::vec3& v) {
	return physim::math::vec3(v.x,v.y,v.z);
}
