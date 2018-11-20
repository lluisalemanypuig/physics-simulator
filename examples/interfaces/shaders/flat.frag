#version 330 core

uniform bool box;

in vec3 frag_normal;

out vec4 frag_color;

void main() {
	if (box) {
		frag_color = vec4(1.0,0.0,0.0,1.0);
	}
	else {
		frag_color = vec4(0.8,0.8,0.8,1.0)*normalize(frag_normal).z;
	}
} 
