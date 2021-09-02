#ifndef MAP_H
#define MAP_H

#include "chunk.h"

typedef struct map_t {
    i32 size;
    chunk_t ***chunks[4]; /* [trigonometric][z][x] */
    chunk_t *chunk00;
} map_t;

map_t * map_create(void);

void map_destroy(map_t *);
void map_resize(map_t *, i32 size);

void map_set_chunk(i32 z, i32 x, chunk_t *chunk, map_t *map);
chunk_t * map_get_chunk(i32 z, i32 x, map_t *map);

cube_t * map_get_cube(i32 x, i32 y, i32 z, map_t *map);
void map_remove_cube(map_t *map, cube_t cube);

#endif

