#ifndef OPENGL_CROSS_H
#define OPENGL_CROSS_H

#include "shader.h"
#include "opengl.h"

typedef struct cross_t {
    GLuint vao;
    GLuint vbo;
    shader_program_t shader_program;
} cross_t;

cross_t * cross_create();
void cross_destroy(cross_t *);
void cross_draw(cross_t *);

#endif //OPENGL_CROSS_H
