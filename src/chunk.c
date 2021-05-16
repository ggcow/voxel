#include "chunk.h"
#include "opengl.h"
#include <math.h>

static bool map_verify(i32 x, i32 y, i32 z, chunk_t *chunk);
static u32 map_get(i32 x, i32 y, i32 z, chunk_t *chunk);
static void map_set(i32 x, i32 y, i32 z, u32 index, chunk_t *chunk);

static void chunk_gen_buffer(chunk_t *chunk);

static bool equation(i32 x, i32 y, i32 z) {
    f32 e = sin(M_PI*sqrt(x*x+z*z)/100);
    return y <= fabsf(e);
}

chunk_t * chunk_create(i32 z, i32 x) {
    chunk_t *chunk = allocate(sizeof(chunk_t));
    chunk->x = x;
    chunk->z = z;
    glGenBuffers(1, &chunk->vbo);
    chunk->map = NULL;
    buffer_init(chunk->cube_buffer);
    buffer_init(chunk->data_buffer);
    chunk->cube_buffer.index = 1;
    return chunk;
}

void chunk_destroy(chunk_t *chunk) {
    buffer_terminate(chunk->data_buffer);
    buffer_terminate(chunk->cube_buffer);
    if (chunk->map) deallocate(chunk->map-MAP_HEIGHT/2);
    deallocate(chunk);
}

void chunk_gen_map(chunk_t *chunk) {
    if (chunk->map) return;
    chunk->map = allocate(sizeof(u32) * CHUNK_SIZE * CHUNK_SIZE * MAP_HEIGHT);
    chunk->map += MAP_HEIGHT/2;

    for (i32 i=0; i<CHUNK_SIZE; i++) {
        for (i32 j=-MAP_HEIGHT/2; j<MAP_HEIGHT/2; j++) {
            buffer_check_size(chunk->cube_buffer, CHUNK_SIZE);
            for (i32 k=0; k<CHUNK_SIZE; k++) {
                if (equation(i*CHUNK_SIZE, j, k*CHUNK_SIZE)) {
                    map_set(i, j, k, chunk->cube_buffer.index, chunk);
                    buffer_push(chunk->cube_buffer, ((cube_t) {i, j, k}));
                } else {
                    map_set(i, j, k, 0, chunk);
                }
            }
        }
    }
    chunk_gen_buffer(chunk);
}

static void chunk_gen_buffer(chunk_t *chunk) {
    for (int index=1; index<chunk->cube_buffer.index; index++) {
        buffer_check_size(chunk->data_buffer, 2*3*4);

        i32 x = chunk->cube_buffer.data[index].x;
        i32 y = chunk->cube_buffer.data[index].y;
        i32 z = chunk->cube_buffer.data[index].z;

        for (int i=-1; i<2; i+=2) {
            if (!map_get(x+i, y, z, chunk)) {
                buffer_push(chunk->data_buffer, x + chunk->x * CHUNK_SIZE + (i+1)/2);
                buffer_push(chunk->data_buffer, y);
                buffer_push(chunk->data_buffer, z + chunk->z * CHUNK_SIZE);
                buffer_push(chunk->data_buffer, 0);
            }
            if (!map_get(x, y+i, z, chunk)) {
                buffer_push(chunk->data_buffer, x + chunk->x * CHUNK_SIZE);
                buffer_push(chunk->data_buffer, y + (i+1)/2);
                buffer_push(chunk->data_buffer, z + chunk->z * CHUNK_SIZE + 1);
                buffer_push(chunk->data_buffer, 1);
            }
            if (!map_get(x, y, z+i, chunk)) {
                buffer_push(chunk->data_buffer, x + chunk->x * CHUNK_SIZE);
                buffer_push(chunk->data_buffer, y);
                buffer_push(chunk->data_buffer, z + chunk->z * CHUNK_SIZE +(i+1)/2);
                buffer_push(chunk->data_buffer, 2);
            }
        }
    }
}

static bool map_verify(i32 x, i32 y, i32 z, chunk_t *chunk) {
    return -MAP_HEIGHT/2 <= y && y < MAP_HEIGHT/2
            && 0 <= z && z < CHUNK_SIZE
            && 0 <= x && x < CHUNK_SIZE;
}

static u32 map_get(i32 x, i32 y, i32 z, chunk_t *chunk) {
    return map_verify(x, y, z, chunk) ?
    chunk->map[CHUNK_SIZE * MAP_HEIGHT * x + MAP_HEIGHT * z + y] : 0;
}

static void map_set(i32 x, i32 y, i32 z, u32 index, chunk_t *chunk) {
    chunk->map[CHUNK_SIZE * MAP_HEIGHT * x + MAP_HEIGHT * z + y] = index;
}