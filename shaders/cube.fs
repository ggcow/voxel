#version 330 core

out vec4 color;
in vec2 uv;
flat in vec3 normal;
in vec3 pos;
flat in int targeted;
flat in int texture_index;

uniform sampler2D texture_map;
uniform uint texture_count;

const float ambient = .1;
in vec3 light_source;


void main() {
	color = texture2D(texture_map, vec2(uv.x, (uv.y + texture_index) / texture_count));
	if (targeted == 1)
	if (min(uv.x, uv.y) < .01f || max(uv.x, uv.y) > .99f) {
		color = vec4(1, 1, 1, 1);
		return;
	}
	vec3 surface_to_light = light_source - pos;
	float d = length(surface_to_light);
	float brightness = dot(normal, surface_to_light) / d;
	brightness = max(1 - d/40, 0) * clamp(brightness, 0, 1);
	color = vec4(min(ambient + brightness, 1) * color.rgb, color.a);
}
