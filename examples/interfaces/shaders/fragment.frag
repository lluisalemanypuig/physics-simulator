#version 330 core

in vec3 frag_normal;

out vec4 frag_color;

void main() {
	frag_color = vec4(0.8,0.8,0.8,1.0)*normalize(frag_normal).z;
} 
