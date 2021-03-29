#ifndef DRAW_H
#define DRAW_H

#include "common.h"
#include "renderer.h"
#include "map.h"

struct draw_thread_data_t {
	GLint *buffer;
	u64 buffer_size;
	u64 buffer_index;
	GLuint *indices;
	u64 indices_size;
	u64 indices_index;

	struct map_t *map;

	i32 id;
	i32 *working;
	u32 *progress;
	u64 start;
	u64 end;
};


void draw_cubes(struct renderer_t *renderer, struct map_t *map);





#endif