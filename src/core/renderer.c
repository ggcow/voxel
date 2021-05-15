#define _POSIX_C_SOURCE 200809L


#include <stdio.h>

#include "core/renderer.h"
#include "core/shader.h"
#include "draw.h"

static bool setup(renderer_t *renderer, map_t *map);

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
    char vertex_shader_path[200];
    char fragment_shader_path[200];
    strcpy(vertex_shader_path, ROOT_FOLDER);
    strcpy(fragment_shader_path, ROOT_FOLDER);
    strcat(vertex_shader_path, "/shaders/vertex.vs");
    strcat(fragment_shader_path, "/shaders/fragment.fs");
    char *vertex_shader_code = shader_load_from_file(vertex_shader_path);
    char *fragment_shader_code = shader_load_from_file(fragment_shader_path);

    if (vertex_shader_code[0] == '\0' || fragment_shader_code[0] == '\0') {
        return FALSE;
    }

	renderer->program = create_shader_program(
            vertex_shader_code,
            fragment_shader_code
    );

	deallocate(vertex_shader_code);
	deallocate(fragment_shader_code);

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



