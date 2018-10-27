#version 330 core

in vec3 fragNormal;
out vec4 fragColor;

uniform vec4 color;

void main() {
	fragColor = color;
}
