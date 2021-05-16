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

void map_chunk_set(i32 z, i32 x, chunk_t *, map_t *);
chunk_t * map_chunk_get(i32 z, i32 x, map_t *);

#endif

