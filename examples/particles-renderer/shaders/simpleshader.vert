#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 fragNormal;

uniform bool is_point;
uniform mat4 projection, modelview;
uniform mat3 normalMatrix;

void main() {
	gl_Position = projection*modelview*vec4(position, 1.0f);
	if (is_point) {
		gl_PointSize = 10.0f;
	}
	else {
		fragNormal = normalMatrix*normal;
	}
}

