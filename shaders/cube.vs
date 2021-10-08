#version 330 core

const mat3x3 M[6] = mat3x3[](
                    mat3x3(0,0,1,0,1,0, 0,0,0),
                    mat3x3(1,0,0,0,0,1, 0,0,0),
                    mat3x3(-1,0,0,0,1,0, 1,0,0),
                    mat3x3(0,0,-1,0,1,0, 1,0,1),
                    mat3x3(1,0,0,0,0,-1, 0,1,1),
                    mat3x3(1,0,0,0,1,0, 0,0,1));

layout(location = 0) in ivec2 vertex_position;
layout(location = 1) in ivec4 vertex_data;

uniform mat4 MVP;
uniform vec3 eye;
uniform ivec3 target;

out vec2 uv;
flat out vec3 normal;
out vec3 pos;
out vec3 light_source;
flat out int texture_index;
flat out int targeted;

void main(){
    int index = vertex_data.w & 7;
    pos = M[index] * ivec3(vertex_position, 1) + vertex_data.xyz;
	gl_Position =  MVP * vec4(pos, 1);
	uv = (vertex_position+1/2);
    texture_index = vertex_data.w >> 3;
    normal = (vec3(1, 1, 1) - abs(M[index] * ivec3(1, 1, 0))) * sign(index*2-5);
    light_source = eye;
    targeted = int(target == vertex_data.xyz);
}