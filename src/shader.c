#include "shader.h"
#include "common.h"

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

shader_program_t shader_program_make(const GLchar *vertex_shader_source,
                                     const GLchar *fragment_shader_source) {
    GLuint program = 0;
    GLuint fragment_shader = 0;
    GLuint vertex_shader = 0;
    vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_source);
    if (!vertex_shader) {
        goto error;
    }

    fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
    if (!fragment_shader) {
        goto error;
    }

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

    shader_program_t shader_program = {
            program,
            vertex_shader,
            fragment_shader
    };
    return shader_program;

    error:
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