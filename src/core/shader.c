#include "core/shader.h"

GLuint _create_shader(GLenum shader_type, const GLchar *shader_source) {
	GLuint shader = glCreateShader(shader_type);
	if (shader == 0) {
		goto exit;
	}

	glShaderSource(shader, 1, &shader_source, NULL);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		GLint log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
		GLchar *log = allocate(sizeof(GLchar) * log_length);
		glGetShaderInfoLog(shader, log_length, &log_length, log);
		log_error("Shader compilation failed:\n%s", log);
		deallocate(log);
		glDeleteShader(shader);
		shader = 0;
	}

exit:
	return shader;
}

GLuint _create_shader_program(const GLchar *vertex_shader_source, const GLchar *fragment_shader_source) {
	GLuint shader_program = 0;

	GLuint vertex_shader = _create_shader(GL_VERTEX_SHADER, vertex_shader_source);
	if (vertex_shader == 0) {
		goto exit;
	}

	GLuint fragment_shader = _create_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
	if (fragment_shader == 0) {
		goto delete_vertex_shader;
	}

	shader_program = glCreateProgram();
	if (shader_program == 0) {
		goto delete_fragment_shader;
	}

	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	glLinkProgram(shader_program);

	GLint success;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (success != GL_TRUE) {
		GLint log_length;
		glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &log_length);
		GLchar *log = allocate(sizeof(GLchar) * log_length);
		glGetProgramInfoLog(shader_program, log_length, &log_length, log);
		log_error("Shader program linking failed:\n%s", log);
		deallocate(log);
		glDeleteProgram(shader_program);
		shader_program = 0;
	}

delete_fragment_shader:
	glDeleteShader(fragment_shader);
delete_vertex_shader:
	glDeleteShader(vertex_shader);
exit:
	return shader_program;
}