#include "texture.h"
#include "stb_image.h"
#include "opengl.h"

static void texture_load(GLubyte *array, const char *path);

void texture_setup(void)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    GLubyte *data = allocate(sizeof (GLubyte) * TEXTURE_SIZE * TEXTURE_SIZE * 3 * TEXTURE_COUNT);
    texture_load(data, "../assets/dirt.png");
    texture_load(data, "../assets/top.png");
    texture_load(data, "../assets/side.png");

    glTexImage2D(GL_TEXTURE_2D,
                 0, GL_RGB,
                 TEXTURE_SIZE, TEXTURE_SIZE * TEXTURE_COUNT,
                 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    deallocate(data);
}

static void texture_load(GLubyte *array, const char *path)
{
    static usize offset = 0;
    GLubyte *data = stbi_load(path, NULL, NULL, NULL, STBI_rgb);
    if (data) {
        memmove(array + offset, data, TEXTURE_SIZE * TEXTURE_SIZE * 3);
        stbi_image_free(data);
        offset += TEXTURE_SIZE * TEXTURE_SIZE * 3;
    } else {
        log_error("Failed to load texture %s", path);
        if (stbi_failure_reason())
            log_error(stbi_failure_reason());
    }
}