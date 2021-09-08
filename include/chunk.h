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

    enum block *map;
    buffer_type(cube_t) cube_buffer;

    GLuint vbo;
    buffer_type(GLint) data_buffer;

    bool loaded;
} chunk_t;

typedef struct map_t map_t;

chunk_t * chunk_create(i32 z, i32 x);
void chunk_destroy(chunk_t *);
void chunk_free_map(chunk_t *);
void chunk_free_buffer(chunk_t *);
void chunk_gen_map(chunk_t *);
void chunk_gen_buffer(chunk_t *, map_t *);
void chunk_load(chunk_t *);
void chunk_unload(chunk_t *);
void chunk_reload(chunk_t *, map_t *);
enum block chunk_get_cube(i32 x, i32 y, i32 z, chunk_t *);
void chunk_set_cube(cube_t, chunk_t *);
void chunk_remove_cube(i32 x, i32 y, i32 z, chunk_t *);
void chunk_add_cube(cube_t cube, chunk_t *);

#endif
