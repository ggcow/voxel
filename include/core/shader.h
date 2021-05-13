#ifndef SHADER_H
#define SHADER_H

#include "core/common.h"
#include "opengl.h"


GLuint _create_shader(GLenum shader_type, const GLchar *shader_source);


GLuint _create_shader_program(const GLchar *vertex_shader_source, const GLchar *fragment_shader_source);


#endif
