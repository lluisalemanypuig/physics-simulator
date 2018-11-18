#include <base/shader.hpp>

// C++ includes
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

shader::shader() {
	ID = 0;
}

shader::~shader() {

}

// MODIFIERS

bool shader::init(const string& vertex_file, const string& fragment_file) {
	// 1. retrieve the vertex/fragment source code from filePath
	string vertex_code;
	string fragment_code;
	ifstream vertex_shader_file;
	ifstream fragment_shader_file;

	// ensure ifstream objects can throw
	// exceptions when reading
	vertex_shader_file.exceptions(ifstream::badbit);
	fragment_shader_file.exceptions(ifstream::badbit);

	int success = 1;

	try {
		// open files
		vertex_shader_file.open(vertex_file);
		fragment_shader_file.open(fragment_file);

		if (not vertex_shader_file.is_open()) {
			throw new ifstream::failure("Could not open vertex shader file.");
		}
		if (not fragment_shader_file.is_open()) {
			throw new ifstream::failure("Could not open fragment shader file.");
		}

		// read file's buffer contents into streams
		stringstream vertex_shader_stream, fragment_shader_stream;
		vertex_shader_stream << vertex_shader_file.rdbuf();
		fragment_shader_stream << fragment_shader_file.rdbuf();

		// close file handlers
		vertex_shader_file.close();
		fragment_shader_file.close();

		// convert stream into string
		vertex_code = vertex_shader_stream.str();
		fragment_code = fragment_shader_stream.str();
	}
	catch (ifstream::failure e) {
		cerr << "shader::init - Error while reading shader files." << endl;
		cerr << "    Exception: " << string(e.what()) << endl;
		success = 0;
	}

	if (success == 0) {
		cerr << "    Initialisation of shader program aborted." << endl;
		return false;
	}

	const char *vertex_shader_code = vertex_code.c_str();
	const char *fragment_shader_code = fragment_code.c_str();

	// 2. compile shaders
	char info_buf[512];

	// vertex shader
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertex_shader_code, nullptr);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (success == 0) {
		glGetShaderInfoLog(vertex, 512, nullptr, info_buf);
		cerr << "shader::init - Error:" << endl;
		cerr << "    Vertex shader compilation failed." << endl;
		return false;
	}

	// fragment shader
	unsigned int fragment = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(fragment, 1, &fragment_shader_code, nullptr);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (success == 0) {
		glGetShaderInfoLog(fragment, 512, nullptr, info_buf);
		cerr << "shader::init - Error:" << endl;
		cerr << "    Vertex shader compilation failed." << endl;
		cerr << "    Log: " << info_buf << endl;
		return false;
	}

	// create shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (success == 0)
	{
		glGetProgramInfoLog(ID, 512, nullptr, info_buf);
		cerr << "shader::init - Error:" << endl;
		cerr << "    Fragment shader compilation failed." << endl;
		cerr << "    Log: " << info_buf << endl;
		return false;
	}

	// delete shaders as they're linked into
	// the program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return true;
}

void shader::clear() {
	if (ID != 0) {
		glDeleteProgram(ID);
		ID = 0;
	}
}

// SETTERS

void shader::set_bool(const string& name, bool value) const {
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniform1i(loc, value);
}

void shader::set_int(const string& name, int value) const {
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniform1i(loc, value);
}

void shader::set_float(const string& name, float value) const {
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniform1f(loc, value);
}

void shader::set_vec2(const string& name, const vec2& v) const {
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniform2f(loc, v.x, v.y);
}

void shader::set_vec3(const string& name, const vec3& v) const {
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniform3f(loc, v.x, v.y, v.z);
}

// GETTERS

GLuint shader::get_id() const {
	return ID;
}

// OTHERS

void shader::use() {
	glUseProgram(ID);
}
