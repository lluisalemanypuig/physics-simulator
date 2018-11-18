#pragma once

// C++ includes
#include <string>

// base includes
#include <base/include_gl.hpp>

// physim includes
#include <physim/math/vec2.hpp>
#include <physim/math/vec3.hpp>
typedef physim::math::vec2 vec2;
typedef physim::math::vec3 vec3;

class shader {
	private:
		/// The program ID.
		unsigned int ID;

	public:
		/// Default constructor.
		shader();
		/// Destructor constructor.
		~shader();

		// MODIFIERS

		/**
		 * @brief Initialise a shader program.
		 *
		 * Loads a vertex and shader GLSL programs found in @e vertex and
		 * @e fragment files.
		 * @param vertex Filename of the vertex shader program.
		 * @param fragment Filename of the fragment shader program.
		 * @returns Returns true on success. Returns false if otherwise.
		 */
		bool init(const std::string& vertex, const std::string& fragment);

		/// Frees the shader program's memory.
		void clear();

		// SETTERS

		/// Sets a Boolean value to the specified uniform.
		void set_bool(const std::string& name, bool value) const;
		/// Sets an integer value to the specified uniform.
		void set_int(const std::string& name, int value) const;
		/// Sets a floating-point value to the specified uniform.
		void set_float(const std::string& name, float value) const;
		/// Sets a vec2 value to the specified uniform.
		void set_vec2(const std::string& name, const vec2& v) const;
		/// Sets a vec3 value to the specified uniform.
		void set_vec3(const std::string& name, const vec3& v) const;

		// GETTERS

		GLuint get_id() const;

		// OTHERS

		/**
		 * @brief Use/activate the shader.
		 *
		 * Calls glUseProgram()
		 */
		void use();
};
