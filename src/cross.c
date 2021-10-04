#include "cross.h"
#include "common.h"

void * memcpy(void *, const void *, unsigned long);

cross_t * cross_create()
{
    cross_t *cross = allocate(sizeof *cross);
    glGenVertexArrays(1, &(cross->vao));
    glGenBuffers(1, &(cross->vbo));
    cross->shader_program = shader_program_make(
            "/shaders/cross.vs",
            "/shaders/cross.fs"
            );
    float w = .04f, h = .04f;
    GLfloat data[] = {
            -w*.1f, -h, -w*.1f, h,
            w*.1f, h, w*.1f, -h,
            -w, -h*.1f, -w, h*.1f,
            w, h*.1f, w, -h*.1f
    };

    glBindBuffer(GL_ARRAY_BUFFER, cross->vbo);
    glBindVertexArray(cross->vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glBufferData(GL_ARRAY_BUFFER, sizeof data, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return cross;
}

void cross_destroy(cross_t *cross)
{
    glDisableVertexArrayAttrib(cross->vao, 0);
    glDeleteBuffers(1, &cross->vbo);
    glDeleteVertexArrays(1, &cross->vao);
    shader_program_terminate(cross->shader_program);
    deallocate(cross);
    log_debug("Cross destroyed");
}

void cross_draw(cross_t *cross)
{
    glBindVertexArray(cross->vao);
    glBindBuffer(GL_ARRAY_BUFFER, cross->vbo);
    glUseProgram(cross->shader_program.program);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}