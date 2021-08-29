#version 330 core

out vec4 color;
in vec2 uv;
flat in int texture_index;

uniform sampler2D texture_map;
uniform uint texture_count;

void main() {
	color = texture2D(texture_map, vec2(uv.x, (uv.y + texture_index) / texture_count));
}
