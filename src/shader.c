#include "shader.h"
#include "common.h"
#include "string.h"

static GLuint create_shader(GLenum shader_type, const GLchar *shader_source);

char * shader_load_from_file(char *path) {
    FILE * f = fopen(path, "r");
    if (!f) {
        log_error("Could not find shader file : %s", path);
        perror (NULL);
        return "";
    }
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    rewind(f);
    char *shader = callocate(length+1);
    fread(shader, 1, length, f);
    fclose(f);
    return shader;
}

static GLuint create_shader(GLenum shader_type, const GLchar *shader_source) {
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

shader_program_t shader_program_make(const char *vertex_shader_rel_path,
                                     const char *fragment_shader_rel_path) {
    GLuint program = 0;
    GLuint fragment_shader = 0;
    GLuint vertex_shader = 0;

    char vertex_shader_path[200];
    char fragment_shader_path[200];
    strcpy(vertex_shader_path, ROOT_FOLDER);
    strcpy(fragment_shader_path, ROOT_FOLDER);
    strcat(vertex_shader_path, vertex_shader_rel_path);
    strcat(fragment_shader_path, fragment_shader_rel_path);
    char *vertex_shader_code = shader_load_from_file(vertex_shader_path);
    char *fragment_shader_code = shader_load_from_file(fragment_shader_path);

    if (!*vertex_shader_code || !*fragment_shader_code) {
        goto error;
    }

    vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_code);
    if (!vertex_shader) {
        goto error;
    }

    fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_code);
    if (!fragment_shader) {
        goto error;
    }

    deallocate(vertex_shader_code);
    deallocate(fragment_shader_code);

    program = glCreateProgram();
    if (!program) {
        goto error;
    }

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success != GL_TRUE) {
        GLint log_length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        GLchar *log = allocate(sizeof(GLchar) * log_length);
        glGetProgramInfoLog(program, log_length, &log_length, log);
        log_error("Shader program linking failed:\n%s", log);
        deallocate(log);
        glDeleteProgram(program);
        program = 0;
        goto error;
    }

    return (shader_program_t) {
            program,
            vertex_shader,
            fragment_shader
    };


    error:
    deallocate(vertex_shader_code);
    deallocate(fragment_shader_code);
    if (fragment_shader) glDeleteShader(fragment_shader);
    if (vertex_shader) glDeleteShader(vertex_shader);
    return (shader_program_t) {.program=0};
}

void shader_program_terminate(shader_program_t shader_program) {
    glDetachShader(shader_program.program, shader_program.vertex_shader);
    glDetachShader(shader_program.program, shader_program.fragment_shader);
    glDeleteShader(shader_program.vertex_shader);
    glDeleteShader(shader_program.fragment_shader);
    glDeleteProgram(shader_program.program);
}