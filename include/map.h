#ifndef MAP_H
#define MAP_H

#include "chunk.h"

struct map_t {
    i32 size;
    chunk_t ***chunks[4];
    chunk_t *chunk00;
};

map_t * map_create(void);

void map_destroy(map_t *);
void map_resize(map_t *, i32 size);

void map_set_chunk(i32 z, i32 x, chunk_t *, map_t *);
chunk_t * map_get_chunk(i32 z, i32 x, map_t *);

enum block map_get_cube(i32 x, i32 y, i32 z, map_t *);
void map_remove_cube(i32 x, i32 y, i32 z, map_t *);
void map_add_cube(cube_t, map_t *);

#endif

