#ifndef CHUNK_H
#define CHUNK_H

#include "common.h"
#include "opengl.h"
#include "buffer.h"
#include "cube.h"

#define CHUNK_SIZE 16
#define MAP_HEIGHT 256

define_buffer_type(cube_t);

typedef struct chunk_t {
    i32 x;
    i32 z;

    u32 *map;
    buffer_type(cube_t) cube_buffer;

    GLuint vbo;
    buffer_type(GLint) data_buffer;
} chunk_t;

chunk_t * chunk_create(i32 z, i32 x);
void chunk_destroy(chunk_t *chunk);
void chunk_gen_map(chunk_t *chunk);
void chunk_gen_buffer(chunk_t *chunk);

#endif