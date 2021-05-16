#include "map.h"

static i32 abs(i32 x) {
    return x<0?-x:x;
}

map_t * map_create(void) {
    map_t *map = callocate(sizeof(map_t));
    map->size = 3;
    for (u32 i=0; i<4; i++) {
        map->chunks[i] = callocate((map->size/2+1) * sizeof(chunk_t ***));
        for (u32 j=0; j<map->size/2+1; j++) {
            map->chunks[i][j] = callocate(map->size/2 * sizeof(chunk_t **));
        }
    }
    for (i32 x=-map->size/2; x<=map->size/2; x++) {
        for (i32 z=-map->size/2; z<=map->size/2; z++) {
            map_chunk_set(z, x, chunk_create(x, z), map);
        }
    }
    chunk_gen_map(map->chunk00);
    chunk_gen_buffer(map->chunk00);
    glBindBuffer(GL_ARRAY_BUFFER, map->chunk00->vbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_INT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(1, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return map;
}

void map_destroy(map_t *map) {
    for (u32 i=0; i<4; i++) {
        for (u32 j=0; j<map->size/2+1; j++) {
            for (u32 k=0; k<map->size/2; k++) {
                if (map->chunks[i][j][k]) {
                    chunk_destroy(map->chunks[i][j][k]);
                } else log_error("no chun in %u %u %u", i, j, k);
            }
            deallocate(map->chunks[i][j]);
        }
        deallocate(map->chunks[i]);
    }
    deallocate(map);
}

void map_resize(map_t *map, i32 size) {
    size |= 1;
    map->size = size;
    for (i32 i=0; i<4; i++) {
        map->chunks[i] = reallocate(map->chunks[i], (map->size/2+1) * sizeof(chunk_t **));
        for (u32 j=0; j<size/2+1; j++) {
            map->chunks[i][j] = reallocate(map->chunks[i][j], map->size/2 * sizeof(chunk_t *));
        }
    }
}

void map_chunk_set(i32 z, i32 x, chunk_t *chunk, map_t *map) {
    i32 index = z >= 0 && x > 0 ? 1 :
                z < 0 && x >= 0 ? 2 :
                z <= 0 && x < 0 ? 3 :
                z > 0 && x <= 0 ? 4 : 0;
    if (!index) map->chunk00 = chunk;
    else map->chunks[index-1][index&1?abs(z):abs(x)][(index&1?abs(x):abs(z))-1] = chunk;
}

chunk_t * map_chunk_get(i32 z, i32 x, map_t *map) {
    i32 index = z >= 0 && x > 0 ? 1 :
                z < 0 && x >= 0 ? 2 :
                z <= 0 && x < 0 ? 3 :
                z > 0 && x <= 0 ? 4 : 0;
    if (!index) return map->chunk00;
    return map->chunks[index-1][abs(z)][abs(x)-1];
}

