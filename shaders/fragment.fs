#version 330 core

out vec3 color;
in vec3 fragment;

void main() {
	color = fragment.xyz/100;
}
