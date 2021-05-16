#version 330 core

out vec3 color;
in vec3 fragment;

const vec3 top_color = vec3(55, 191, 11);
const vec3 bot_color = vec3(143, 152, 144);

void main() {
	float t = (fragment.y+128)/255;
	color = (t*top_color+(1-t)*bot_color)/255;
}
