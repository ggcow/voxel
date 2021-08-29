#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"
#include "player.h"
#include "event.h"
#include "opengl.h"
#include "buffer.h"
#include "matrix.h"
#include "chunk.h"
#include "shader.h"

typedef struct render_t {
    shader_program_t shader_program;
	GLuint vao;
	GLuint vbo;
	buffer_type(GLint) vertex_buffer;
} renderer_t;



renderer_t * renderer_create();
void renderer_destroy(renderer_t *renderer);

void renderer_draw(renderer_t *renderer, player_t *player, matrix_t *mvp);

#endif