#version 330 core

out vec3 color;
in vec3 fragment;

void main() {
	color = mod(fragment.xyz*fragment.xyz, 100)/100;
}
