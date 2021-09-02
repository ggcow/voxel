#version 330 core

const mat2x3 M[6] = mat2x3[](
                    mat2x3(0,0,1,0,1,0),
                    mat2x3(1,0,0,0,0,1),
                    mat2x3(-1,0,0,0,1,0),
                    mat2x3(0,0,-1,0,1,0),
                    mat2x3(1,0,0,0,0,-1),
                    mat2x3(1,0,0,0,1,0));

layout(location = 0) in ivec2 vertex_position;
layout(location = 1) in ivec4 vertex_data;

uniform mat4 MVP;
uniform vec3 eye;

out vec2 uv;
flat out vec3 normal;
out vec3 pos;
out vec3 light_source;
flat out int texture_index;

void main(){
    int index = vertex_data.w & 7;
    pos = M[index] * vertex_position + vertex_data.xyz;
	gl_Position =  MVP * vec4(pos, 1);
	uv = (vertex_position+1/2);
    texture_index = vertex_data.w >> 3;
    normal = (vec3(1, 1, 1) - abs(M[index] * vec2(1, 1))) * sign(index*2-5);
//    normal = vec3(0, 1, 0);
    light_source = eye;
}