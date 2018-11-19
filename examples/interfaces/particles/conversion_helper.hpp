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
