#pragma once

// base includes
#include <base/model/rendered_model.hpp>
#include <base/shader/shader.hpp>

namespace shader_helper {

	void set_materials_shader(const rendered_model& M, shader& S);

	void activate_textures(const rendered_model& M, shader& S);

} // -- namespace shader_helper
