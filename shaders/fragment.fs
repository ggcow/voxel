#version 330 core

out vec3 color;
in vec3 fragmentColor;

void main() {
	color = mod(fragmentColor,20)/20;
}
