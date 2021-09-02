#include "chunk.h"
#include "opengl.h"
#include "texture.h"
#include <math.h>
#include <map.h>


static bool equation(i32 x, i32 y, i32 z)
{
//    f32 e = 30*sin(M_PI*sqrt(x*x+z*z)/85);
    return x*x+y*y+z*z <= 100;
//    return y  <= fabsf(e)/30000;
}

chunk_t * chunk_create(i32 z, i32 x)
{
    chunk_t *chunk = callocate(sizeof(chunk_t));
    chunk->x = x;
    chunk->z = z;
    glGenBuffers(1, &chunk->vbo);
    return chunk;
}

void chunk_destroy(chunk_t *chunk)
{
    chunk_free_buffer(chunk);
    chunk_free_map(chunk);
    deallocate(chunk);
}

void chunk_free_map(chunk_t *chunk)
{
    buffer_terminate(chunk->cube_buffer);
    if (chunk->map) deallocate(chunk->map-MAP_HEIGHT/2);
    chunk->map = NULL;
    chunk->cube_buffer.index = 1;
}

void chunk_free_buffer(chunk_t *chunk)
{
    buffer_terminate(chunk->data_buffer);
    chunk->data_buffer.index = 0;
}

void chunk_gen_map(chunk_t *chunk)
{
    if (chunk->cube_buffer.index > 1) return;
    buffer_init(chunk->cube_buffer);
    chunk->cube_buffer.index = 1;
    chunk->map = allocate(sizeof(u32) * CHUNK_SIZE * CHUNK_SIZE * MAP_HEIGHT);
    chunk->map += MAP_HEIGHT/2;

    for (i32 i=0; i<CHUNK_SIZE; i++) {
        for (i32 j=-MAP_HEIGHT/2; j<MAP_HEIGHT/2; j++) {
            buffer_check_size(chunk->cube_buffer, CHUNK_SIZE);
            for (i32 k=0; k<CHUNK_SIZE; k++) {
                if (equation(i+chunk->x*CHUNK_SIZE, j, k+chunk->z*CHUNK_SIZE)) {
                    chunk_set_cube(i, j, k, chunk->cube_buffer.index, chunk);
                    buffer_push(chunk->cube_buffer, ((cube_t) {i, j, k}));
                } else {
                    chunk_set_cube(i, j, k, 0, chunk);
                }
            }
        }
    }
}

void chunk_gen_buffer(chunk_t *chunk, map_t *map)
{
    if (chunk->data_buffer.index > 0) return;
    buffer_init(chunk->data_buffer);
    for (int index=1; index<chunk->cube_buffer.index; index++) {
        buffer_check_size(chunk->data_buffer, 2*3*4);

        i32 x = chunk->cube_buffer.data[index].x;
        i32 y = chunk->cube_buffer.data[index].y;
        i32 z = chunk->cube_buffer.data[index].z;

        int grass_side = (!chunk_get_cube(x, y + 1, z, chunk)) * TEXTURE_SIDE << 3;

        for (int i=-1; i<2; i+=2) {
            if (!map_get_cube(x + i, y, z, map)) {
                buffer_push(chunk->data_buffer, x + chunk->x * CHUNK_SIZE +(i+1)/2);
                buffer_push(chunk->data_buffer, y);
                buffer_push(chunk->data_buffer, z + chunk->z * CHUNK_SIZE +(i+1)/2);
                buffer_push(chunk->data_buffer, (i<0?0:3) | grass_side);
            }
            if (!map_get_cube(x, y + i, z, map)) {
                buffer_push(chunk->data_buffer, x + chunk->x * CHUNK_SIZE);
                buffer_push(chunk->data_buffer, y + (i+1)/2);
                buffer_push(chunk->data_buffer, z + chunk->z * CHUNK_SIZE +(i+1)/2);
                buffer_push(chunk->data_buffer, i<0?1:4 | TEXTURE_TOP<<3);
            }
            if (!map_get_cube(x, y, z + i, map)) {
                buffer_push(chunk->data_buffer, x + chunk->x * CHUNK_SIZE -(i-1)/2);
                buffer_push(chunk->data_buffer, y);
                buffer_push(chunk->data_buffer, z + chunk->z * CHUNK_SIZE +(i+1)/2);
                buffer_push(chunk->data_buffer, (i<0?2:5) | grass_side);
            }
        }
    }
}

void chunk_load(chunk_t *chunk)
{
    if (chunk->loaded) return;
    chunk->loaded = TRUE;
    glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLint) * (chunk->data_buffer.index),
                 chunk->data_buffer.data,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void chunk_unload(chunk_t *chunk)
{
    chunk->loaded = FALSE;
    glInvalidateBufferData(chunk->vbo);
}

u32 chunk_get_cube(i32 x, i32 y, i32 z, chunk_t *chunk) {
    chunk_gen_map(chunk);
    return chunk->map[CHUNK_SIZE * MAP_HEIGHT * x + MAP_HEIGHT * z + y];
}

void chunk_set_cube(i32 x, i32 y, i32 z, u32 index, chunk_t *chunk) {
    chunk->map[CHUNK_SIZE * MAP_HEIGHT * x + MAP_HEIGHT * z + y] = index;
}