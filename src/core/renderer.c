#define _POSIX_C_SOURCE 200809L


#include <stdio.h>

#include "core/renderer.h"
#include "core/shader.h"
#include "draw.h"

static bool setup(renderer_t *renderer, map_t *map);

static const GLchar *_2D_VERTEX_SHADER_SOURCE = 

"#version 330 core                                                                                          \n\
                                                                                                            \n\
const mat3 M[3] = mat3[](                                                                                   \n\
                   mat3(0,0,1,0,1,0,-1,0,0),                                                                \n\
                   mat3(1,0,0,0,0,-1,0,-1,0),                                                               \n\
                   mat3(1,0,0,0,1,0,0,0,1));                                                                \n\
                                                                                                            \n\
layout(location = 0) in vec2 vertex_position;                                                               \n\
layout(location = 1) in vec4 vertex_data;                                                                   \n\
                                                                                                            \n\
uniform mat4 MVP;                                                                                           \n\
out vec3 fragmentColor;                                                                                     \n\
                                                                                                            \n\
void main(){                                                                                                \n\
	gl_Position =  MVP * vec4(M[int(vertex_data.w)] * vec3(vertex_position,0)+vertex_data.xyz,1);           \n\
	fragmentColor = vertex_data.xyz;                                                                        \n\
}";


static const GLchar *_2D_FRAGMENT_SHADER_SOURCE = 
"#version 330 core                                                                                          \n\
                                                                                                            \n\
out vec3 color;                                                                                             \n\
in vec3 fragmentColor;                                                                                      \n\
void main() {                                                                                               \n\
	color = mod(fragmentColor,20)/20;                                                                       \n\
//  color = fragmentColor;                                                                                  \n\
}";

void renderer_draw(renderer_t *renderer, player_t *player, matrix_t *mvp) {

	GLuint MatrixID = glGetUniformLocation(renderer->program, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, (GLfloat *) mvp);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP,
                         0,
                         renderer->vertex_buffer.index,
                         renderer->data_buffer.index/4);
}



static bool setup(renderer_t *renderer, map_t *map) {
	renderer->program = _create_shader_program(
		_2D_VERTEX_SHADER_SOURCE,
		_2D_FRAGMENT_SHADER_SOURCE
	);

	if (renderer->program == 0) {
		return FALSE;
	}

	buffer_init(renderer->vertex_buffer);
	buffer_init(renderer->data_buffer);

	draw_cubes(renderer, map);
    buffer_check_size(renderer->vertex_buffer, 8);
    buffer_push(renderer->vertex_buffer, 0); buffer_push(renderer->vertex_buffer, 0);
    buffer_push(renderer->vertex_buffer, 0); buffer_push(renderer->vertex_buffer, 1);
    buffer_push(renderer->vertex_buffer, 1); buffer_push(renderer->vertex_buffer, 1);
    buffer_push(renderer->vertex_buffer, 1); buffer_push(renderer->vertex_buffer, 0);

	glGenVertexArrays(1, &(renderer->vao));
	glGenBuffers(1, &(renderer->vbo));
	glGenBuffers(1, &(renderer->dbo));

	glBindVertexArray(renderer->vao);

	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLint)*(renderer->vertex_buffer.index), renderer->vertex_buffer.data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, renderer->dbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLint)*(renderer->data_buffer.index), renderer->data_buffer.data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_INT, GL_FALSE, 0, (void*)0);
    glVertexAttribDivisor(1, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    renderer_set_clear_color(renderer, 0.0f, 0.0f, 0.0f, 0.0f);
	glClearColor(renderer->clear_color[0],
              renderer->clear_color[1],
              renderer->clear_color[2],
              renderer->clear_color[3]);

	glUseProgram(renderer->program);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return TRUE;
}


renderer_t * renderer_create(map_t *map) {
	renderer_t *renderer = allocate(sizeof(renderer_t));

	if (!setup(renderer, map)) {
		return NULL;
	}

	log_debug("Renderer created");
	return renderer;
}

void renderer_destroy(renderer_t *renderer) {
	buffer_terminate(renderer->vertex_buffer);
	buffer_terminate(renderer->data_buffer);
	deallocate(renderer);
	log_debug("Renderer destroyed");
}

void renderer_set_clear_color(renderer_t *renderer, f32 r, f32 g, f32 b, f32 a) {
	renderer->clear_color[0] = r;
	renderer->clear_color[1] = g;
	renderer->clear_color[2] = b;
	renderer->clear_color[3] = a;
}



