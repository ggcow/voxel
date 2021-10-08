#include <stdio.h>

#include "renderer.h"
#include "texture.h"

static bool setup(renderer_t *renderer);

void renderer_draw(renderer_t *renderer, player_t *player, matrix_t *mvp)
{
    glUseProgram(renderer->shader_program.program);
	GLint MatrixID = glGetUniformLocation(renderer->shader_program.program, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, (GLfloat *) mvp);

    GLint eyeID = glGetUniformLocation(renderer->shader_program.program, "eye");
    glUniform3fv(eyeID, 1, player->eye);

    GLint targetID = glGetUniformLocation(renderer->shader_program.program, "target");
    glUniform3iv(targetID, 1, &player->target.x);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(renderer->vao);
	plist_foreach(player->chunk_list, chunk, chunk_t) {
        glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
        glEnableVertexAttribArray(1);
        glVertexAttribIPointer(1, 4, GL_INT, 0, NULL);
        glVertexAttribDivisor(1, 1);
        glDrawArraysInstanced(GL_TRIANGLE_FAN,
                              0,
                              renderer->vertex_buffer.index,
                              chunk->data_buffer.index / 4);
	}
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static bool setup(renderer_t *renderer)
{
    renderer->shader_program = shader_program_make(
            "/shaders/cube.vs",
            "/shaders/cube.fs");

    if (renderer->shader_program.program == 0) {
        log_error("Error while making shader program");
        return FALSE;
    }

	buffer_init(renderer->vertex_buffer);

    buffer_check_size(renderer->vertex_buffer, 8);
    buffer_push(renderer->vertex_buffer, 0); buffer_push(renderer->vertex_buffer, 0);
    buffer_push(renderer->vertex_buffer, 0); buffer_push(renderer->vertex_buffer, 1);
    buffer_push(renderer->vertex_buffer, 1); buffer_push(renderer->vertex_buffer, 1);
    buffer_push(renderer->vertex_buffer, 1); buffer_push(renderer->vertex_buffer, 0);


	glGenVertexArrays(1, &(renderer->vao));
	glGenBuffers(1, &(renderer->vbo));

	glBindVertexArray(renderer->vao);

	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
	glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr) (sizeof(GLint) * (renderer->vertex_buffer.index)),
              renderer->vertex_buffer.data,
              GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribIPointer(0, 2, GL_INT, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	glClearColor(.5f, .8f, .9f, 1);

	texture_setup();
    GLint texture_count_location = glGetUniformLocation(renderer->shader_program.program, "texture_count");
    glUseProgram(renderer->shader_program.program);
    glUniform1ui(texture_count_location, TEXTURE_COUNT);
    glUseProgram(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LESS);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

	return TRUE;
}


renderer_t * renderer_create(void)
{
	renderer_t *renderer = callocate(sizeof(renderer_t));

	if (!setup(renderer)) {
		return NULL;
	}

	log_debug("Renderer created");
	return renderer;
}

void renderer_destroy(renderer_t *renderer)
{
    glDisableVertexArrayAttrib(renderer->vao, 0);
    glDisableVertexArrayAttrib(renderer->vao, 1);
    glDeleteBuffers(1, &renderer->vbo);
    glDeleteVertexArrays(1, &renderer->vao);
    shader_program_terminate(renderer->shader_program);
	buffer_terminate(renderer->vertex_buffer);
	deallocate(renderer);
	log_debug("Renderer destroyed");
}


