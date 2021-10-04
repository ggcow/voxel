#include "chunk.h"
#include "opengl.h"
#include "texture.h"
#include <math.h>
#include <map.h>

#define ABS(x) (x>0?x:-x)
#define MAX(x, y) (x>y?x:y)
#define MAXX(x, y , z) MAX(ABS(x), MAX(ABS(y), ABS(z)))

static bool equation(i32 x, i32 y, i32 z)
{
//    x-=8;
//    z-=8;
//    f32 e = 30*sin(M_PI*sqrt(x*x+z*z)/85);
    return MAXX(x, y, z) <=4;
//    return y  <= fabsf(e);
//    return y<0;
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
    glDeleteBuffers(1, &chunk->vbo);
    deallocate(chunk);
}

void chunk_free_map(chunk_t *chunk)
{
    buffer_terminate(chunk->cube_buffer);
    if (chunk->map) deallocate(chunk->map-MAP_HEIGHT/2);
    chunk->map = NULL;
}

void chunk_free_buffer(chunk_t *chunk)
{
    buffer_terminate(chunk->data_buffer);
    chunk->data_buffer.index = 0;
}

void chunk_gen_map(chunk_t *chunk)
{
    if (chunk->cube_buffer.data) return;
    buffer_init(chunk->cube_buffer);
    chunk->cube_buffer.index = 1;
    chunk->map = allocate(sizeof(u32) * CHUNK_SIZE * CHUNK_SIZE * MAP_HEIGHT);
    chunk->map += MAP_HEIGHT/2;

    for (i32 i=0; i<CHUNK_SIZE; i++) {
        for (i32 j=-MAP_HEIGHT/2; j<MAP_HEIGHT/2; j++) {
            buffer_check_size(chunk->cube_buffer, CHUNK_SIZE);
            for (i32 k=0; k<CHUNK_SIZE; k++) {
                cube_t cube = {i, j, k, BLOCK_DIRT};
                if (equation(i+chunk->x*CHUNK_SIZE, j, k+chunk->z*CHUNK_SIZE)) {
                    chunk_set_cube(cube, chunk);
                    buffer_push(chunk->cube_buffer, cube);
                } else {
                    cube.type = BLOCK_NONE;
                    chunk_set_cube(cube, chunk);
                }
            }
        }
    }
}

void chunk_gen_buffer(chunk_t *chunk, map_t *map)
{
    if (chunk->data_buffer.data) return;
    log_debug("loading %d cubes", chunk->cube_buffer.index-1);
    buffer_init(chunk->data_buffer);
    for (int index=1; index<chunk->cube_buffer.index; index++) {
        buffer_check_size(chunk->data_buffer, 2*3*4);

        i32 c_x = chunk->cube_buffer.data[index].x;
        i32 y = chunk->cube_buffer.data[index].y;
        i32 c_z = chunk->cube_buffer.data[index].z;

        i32 x = c_x + chunk->x * CHUNK_SIZE;
        i32 z = c_z + chunk->z * CHUNK_SIZE;


        int grass_side = (!chunk_get_cube(c_x, y + 1, c_z, chunk)) * TEXTURE_SIDE << 3;

        for (int i=-1; i<2; i+=2) {
            if (!map_get_cube(x + i, y, z, map)) {
                buffer_push(chunk->data_buffer, x +(i+1)/2);
                buffer_push(chunk->data_buffer, y);
                buffer_push(chunk->data_buffer, z +(i+1)/2);
                buffer_push(chunk->data_buffer, (i<0?0:3) | grass_side);
            }
            if (!map_get_cube(x, y + i, z, map)) {
                buffer_push(chunk->data_buffer, x);
                buffer_push(chunk->data_buffer, y + (i+1)/2);
                buffer_push(chunk->data_buffer, z +(i+1)/2);
                buffer_push(chunk->data_buffer, i<0?1:4 | TEXTURE_TOP<<3);
            }
            if (!map_get_cube(x, y, z + i, map)) {
                buffer_push(chunk->data_buffer, x -(i-1)/2);
                buffer_push(chunk->data_buffer, y);
                buffer_push(chunk->data_buffer, z +(i+1)/2);
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
                 (GLsizeiptr) sizeof(GLint) * (chunk->data_buffer.index),
                 chunk->data_buffer.data,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    buffer_terminate(chunk->data_buffer);
}

void chunk_unload(chunk_t *chunk)
{
    if (!chunk->loaded) return;
    chunk->loaded = FALSE;
    glInvalidateBufferData(chunk->vbo);
}

void chunk_reload(chunk_t *chunk, map_t *map)
{
    chunk_gen_buffer(chunk, map);
    chunk_unload(chunk);
    chunk_load(chunk);
}

enum block chunk_get_cube(i32 x, i32 y, i32 z, chunk_t *chunk)
{
    chunk_gen_map(chunk);
    return chunk->map[CHUNK_SIZE * MAP_HEIGHT * x + MAP_HEIGHT * z + y];
}

void chunk_set_cube(cube_t cube, chunk_t *chunk)
{
    chunk->map[CHUNK_SIZE * MAP_HEIGHT *  cube.x + MAP_HEIGHT *  cube.z +  cube.y] = cube.type;
}

void chunk_remove_cube(i32 x, i32 y, i32 z, chunk_t *chunk)
{
    typeof(chunk->cube_buffer) *buffer = &chunk->cube_buffer;
    int i;
    for (i=1; buffer->data[i].x != x || buffer->data[i].y != y || buffer->data[i].z != z; i++)
        ;
    if (buffer->data[i].x != x && buffer->data[i].y != y && buffer->data[i].z != z) log_error("WTF");
    buffer->data[i] = buffer->data[--buffer->index];
    chunk_set_cube((cube_t) {x, y, z, BLOCK_NONE}, chunk);
}

void chunk_add_cube(cube_t cube, chunk_t *chunk)
{
    buffer_check_size(chunk->cube_buffer, 1);
    buffer_push(chunk->cube_buffer, cube);
    chunk_set_cube(cube, chunk);
}