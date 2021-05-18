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
out vec3 fragment;

void main(){
	gl_Position =  MVP * vec4(M[vertex_data.w] * vertex_position + vertex_data.xyz,1);
	fragment = vertex_data.xyz;
}