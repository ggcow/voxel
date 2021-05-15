#ifndef SHADER_H
#define SHADER_H

#include "opengl.h"

char * shader_load_from_file(char *path);
GLuint create_shader(GLenum shader_type, const GLchar *shader_source);
GLuint create_shader_program(const GLchar *vertex_shader_source, const GLchar *fragment_shader_source);


#endif
