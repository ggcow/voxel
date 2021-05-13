#ifndef DRAW_H
#define DRAW_H

#include "core/common.h"
#include "core/renderer.h"
#include "map.h"


void draw_cubes(renderer_t *renderer, map_t *map);
static void _check_buffer_size(renderer_t *renderer, u32 buffer_added_size, u32 indices_added_size);


#endif