#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in int mat_id;

uniform mat4 projection;
uniform mat4 modelview;
uniform mat3 normal_matrix;

out vec3 frag_normal;
out vec3 frag_pos;
flat out int MAT_ID;

void main() {
	gl_Position = projection*modelview*vec4(position, 1.0);
	frag_normal = normal_matrix*normal;
	frag_pos = vec3(modelview*vec4(position, 1.0));
	MAT_ID = mat_id;
}
