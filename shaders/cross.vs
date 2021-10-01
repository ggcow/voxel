#version 330 core

layout(location = 0) in ivec2 vertex_position;
layout(location = 1) in ivec4 vertex_data;

void main(){
    gl_Position = vec4(vertex_position.xy, 0, 1);
}