#ifndef DRAW_H
#define DRAW_H

#include "common.h"
#include "renderer.h"
#include "map.h"



void draw_cubes(struct renderer_t *renderer, struct map_t *map);



static void _check_buffer_size(struct renderer_t *renderer, u32 buffer_added_size, u32 indices_added_size);








#endif