#include "draw.h"
#include "core/opengl.h"

void draw_cubes(renderer_t *renderer, map_t *map) {
	u32 loading = 0;

	fprintf(stderr, "\n");
	for (int index=1; index<map->cube_buffer.index; index++) {
		if (10.0f*index/map->cube_buffer.index > loading) {
			loading++;
			log_command("A");
			log_command("2K");
			info("Rendering map : [");
			for (int i=0; i<10; i++) {
				if (i<loading) {
					fprintf(stderr, "#");
				} else {
					fprintf(stderr, " ");
				}
			}
			fprintf(stderr, "]\n");
		}

		buffer_check_size(renderer->data_buffer, 2*3*4);

		i32 x = map->cube_buffer.data[index].x;
		i32 y = map->cube_buffer.data[index].y;
		i32 z = map->cube_buffer.data[index].z;

		for (int i=-1; i<2; i+=2) {
			if (!map_g(x+i, y, z)) {
			    buffer_push(renderer->data_buffer, x+(i+1)/2);
                buffer_push(renderer->data_buffer, y);
                buffer_push(renderer->data_buffer, z);
                buffer_push(renderer->data_buffer, 0);
			}
			if (!map_g(x, y+i, z)) {
                buffer_push(renderer->data_buffer, x);
                buffer_push(renderer->data_buffer, y+(i+1)/2);
                buffer_push(renderer->data_buffer, z+1);
                buffer_push(renderer->data_buffer, 1);
			}
			if (!map_g(x, y, z+i)) {
                buffer_push(renderer->data_buffer, x);
                buffer_push(renderer->data_buffer, y);
                buffer_push(renderer->data_buffer, z+(i+1)/2);
                buffer_push(renderer->data_buffer, 2);
			}
		}
	}

	log_command("A");
	log_command("2K");
}
