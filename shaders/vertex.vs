#version 330 core

const mat3 M[3] = mat3[](
                   mat3(0,0,1,0,1,0,-1,0,0),
                   mat3(1,0,0,0,0,-1,0,-1,0),
                   mat3(1,0,0,0,1,0,0,0,1));

layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec4 vertex_data;

uniform mat4 MVP;
out vec3 fragmentColor;

void main(){
	gl_Position =  MVP * vec4(M[int(vertex_data.w)] * vec3(vertex_position,0)+vertex_data.xyz,1);
	fragmentColor = vertex_data.xyz;
}