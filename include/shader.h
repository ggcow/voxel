#ifndef SHADER_H
#define SHADER_H

#include "opengl.h"


typedef struct shader_program_t {
    GLuint program;
    GLuint vertex_shader;
    GLuint fragment_shader;
} shader_program_t;

char * shader_load_from_file(char *path);
shader_program_t shader_program_make(const GLchar *vertex_shader_source, const GLchar *fragment_shader_source);
void shader_program_terminate(shader_program_t shader_program);

#endif
