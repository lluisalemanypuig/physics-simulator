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

bool shader::init(const string& dir, const string& vertex_name, const string& fragment_name) {
	string vertex_file = dir + "/" + vertex_name;
	string fragment_file = dir + "/" + fragment_name;

	#if defined (DEBUG)
	cout << "shader::init - Load shader programs:" << endl;
	cout << "    vertex:   " << vertex_file << endl;
	cout << "    fragment: " << fragment_file << endl;
	#endif

	// 1. retrieve the vertex/fragment source code from filePath
	string vertex_code, fragment_code;
	ifstream vertex_shader_file, fragment_shader_file;

	// ensure ifstream objects can throw
	// exceptions when reading
	vertex_shader_file.exceptions(ifstream::badbit);
	fragment_shader_file.exceptions(ifstream::badbit);

	int success = 1;

	try {
		// open files
		vertex_shader_file.open(vertex_file);
		if (not vertex_shader_file.is_open()) {
			throw new ifstream::failure("Could not open vertex shader file.");
		}

		fragment_shader_file.open(fragment_file);
		if (not fragment_shader_file.is_open()) {
			throw new ifstream::failure("Could not open fragment shader file.");
		}

		#if defined (DEBUG)
		cout << "    Reading buffers..." << endl;
		#endif

		// read file's buffer contents into streams
		stringstream vertex_shader_stream, fragment_shader_stream;
		vertex_shader_stream << vertex_shader_file.rdbuf();
		fragment_shader_stream << fragment_shader_file.rdbuf();

		#if defined (DEBUG)
		cout << "        buffers read" << endl;
		#endif

		// close file handlers
		vertex_shader_file.close();
		fragment_shader_file.close();

		// convert stream into string
		vertex_code = vertex_shader_stream.str();
		fragment_code = fragment_shader_stream.str();

		#if defined (DEBUG)
		cout << "        contents of files stored" << endl;
		#endif
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

	#if defined (DEBUG)
	cout << "    Compile shaders..." << endl;
	#endif

	// 2. compile shaders
	char info_buf[512];

	#if defined (DEBUG)
	cout << "    * vertex shader:" << endl;
	cout << "        - create shader" << endl;
	#endif

	// vertex shader
	unsigned int vertex;
	vertex = glCreateShader(GL_VERTEX_SHADER);

	#if defined (DEBUG)
	cout << "        - set shader source" << endl;
	#endif

	int vertex_length = vertex_code.length();
	glShaderSource(vertex, 1, &vertex_shader_code, &vertex_length);

	#if defined (DEBUG)
	cout << "        - compile source" << endl;
	#endif

	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (success == 0) {
		glGetShaderInfoLog(vertex, 512, nullptr, info_buf);
		cerr << "-------------------------------------" << endl;
		cerr << "shader::init - Error:" << endl;
		cerr << "    Vertex shader compilation failed." << endl;
		cerr << "    Log: " << endl;
		cerr << info_buf << endl;
		cerr << "-------------------------------------" << endl;
		return false;
	}

	#if defined (DEBUG)
	cout << "    * fragment shader..." << endl;
	cout << "        - create shader" << endl;
	#endif

	// fragment shader
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	int fragment_length = fragment_code.length();

	#if defined (DEBUG)
	cout << "        - set shader source" << endl;
	#endif

	glShaderSource(fragment, 1, &fragment_shader_code, &fragment_length);

	#if defined (DEBUG)
	cout << "        - compile source" << endl;
	#endif

	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (success == 0) {
		glGetShaderInfoLog(fragment, 512, nullptr, info_buf);
		cerr << "-------------------------------------" << endl;
		cerr << "shader::init - Error:" << endl;
		cerr << "    Fragment shader compilation failed." << endl;
		cerr << "    Log: " << endl;
		cerr << info_buf << endl;
		cerr << "-------------------------------------" << endl;
		return false;
	}

	#if defined (DEBUG)
	cout << "    Creating program..." << endl;
	#endif

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
		cerr << "-------------------------------------" << endl;
		cerr << "shader::init - Error:" << endl;
		cerr << "    Shader program linkage failed." << endl;
		cerr << "    Log: " << endl;
		cerr << info_buf << endl;
		cerr << "-------------------------------------" << endl;
		return false;
	}

	#if defined (DEBUG)
	cout << "        program created" << endl;
	#endif

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

void shader::set_vec2(const string& name, const glm::vec2& v) const {
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniform2f(loc, v.x, v.y);
}

void shader::set_vec3(const string& name, const glm::vec3& v) const {
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniform3f(loc, v.x, v.y, v.z);
}

void shader::set_mat3(const string& name, const glm::mat3& m) const {
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void shader::set_mat4(const string& name, const glm::mat4& m) const {
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

// GETTERS

GLuint shader::get_id() const {
	return ID;
}

// OTHERS

void shader::bind() const {
	glUseProgram(ID);
}

void shader::release() const {
	glUseProgram(0);
}
