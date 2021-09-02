#include <math.h>
#include "map.h"

#define FLOOR(x) ((int) floorf((f32) x))

static i32 abs(i32 x)
{
    return x<0?-x:x;
}

map_t * map_create(void)
{
    map_t *map = callocate(sizeof(map_t));
    map->size = 1001;
    for (u32 i=0; i<4; i++) {
        map->chunks[i] = callocate((map->size/2+1) * sizeof(chunk_t ***));
        for (u32 j=0; j<map->size/2+1; j++) {
            map->chunks[i][j] = callocate(map->size/2 * sizeof(chunk_t **));
        }
    }
    for (i32 x=-map->size/2; x<=map->size/2; x++) {
        for (i32 z=-map->size/2; z<=map->size/2; z++) {
            map_set_chunk(z, x, chunk_create(z, x), map);
        }
    }
    return map;
}

void map_destroy(map_t *map)
{
    chunk_destroy(map->chunk00);
    for (u32 i=0; i<4; i++) {
        for (u32 j=0; j<map->size/2+1; j++) {
            for (u32 k=0; k<map->size/2; k++) {
                if (map->chunks[i][j][k]) {
                    chunk_destroy(map->chunks[i][j][k]);
                }
            }
            deallocate(map->chunks[i][j]);
        }
        deallocate(map->chunks[i]);
    }
    deallocate(map);
}

void map_resize(map_t *map, i32 size)
{
    //TODO IMPLEMENT MAP_RESIZE AND ADD IT TO CODE
    size |= 1;
    map->size = size;
    for (i32 i=0; i<4; i++) {
        map->chunks[i] = reallocate(map->chunks[i], (map->size/2+1) * sizeof(chunk_t **));
        for (u32 j=0; j<size/2+1; j++) {
            map->chunks[i][j] = reallocate(map->chunks[i][j], map->size/2 * sizeof(chunk_t *));
        }
    }
}

void map_set_chunk(i32 z, i32 x, chunk_t *chunk, map_t *map)
{
    i32 index = z >= 0 && x > 0 ? 1 :
                z < 0 && x >= 0 ? 2 :
                z <= 0 && x < 0 ? 3 :
                z > 0 && x <= 0 ? 4 : 0;
    if (!index) map->chunk00 = chunk;
    else map->chunks[index-1][index&1?abs(z):abs(x)][(index&1?abs(x):abs(z))-1] = chunk;
}

chunk_t * map_get_chunk(i32 z, i32 x, map_t *map)
{
    i32 index = z >= 0 && x > 0 ? 1 :
                z < 0 && x >= 0 ? 2 :
                z <= 0 && x < 0 ? 3 :
                z > 0 && x <= 0 ? 4 : 0;
    if (!index) return map->chunk00;
    return map->chunks[index-1][index&1?abs(z):abs(x)][(index&1?abs(x):abs(z))-1];
}

cube_t * map_get_cube(i32 x, i32 y, i32 z, map_t *map)
{
    chunk_t *chunk = map_get_chunk(FLOOR(z / CHUNK_SIZE), FLOOR(x / CHUNK_SIZE), map);
    u32 index = chunk_get_cube(MOD(x, CHUNK_SIZE), y, MOD(z, CHUNK_SIZE), chunk);
    return index ? chunk->cube_buffer.data + index : NULL;
}

void map_remove_cube(map_t *map, cube_t cube) {

};

#undef FLOOR

