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

		i32 x = map->cube_buffer.data[index].x;
		i32 y = map->cube_buffer.data[index].y;
		i32 z = map->cube_buffer.data[index].z;

		buffer_check_size(renderer->vertex_buffer, 8*3);
		buffer_check_size(renderer->element_buffer, 3*2*6);

		bool face[3][2];
		u32 point[2][2][2];

		for (int i=-1; i<2; i+=2) {
			face[0][(i+1)>>1] = map_v(x+i, y, z)?(map_g(x+i, y, z) > 0):FALSE;
			face[1][(i+1)>>1] = map_v(x, y+i, z)?(map_g(x, y+i, z) > 0):FALSE;
			face[2][(i+1)>>1] = map_v(x, y, z+i)?(map_g(x, y, z+i) > 0):FALSE;
		}

		for (int i=0; i<2; i++) {
			for (int j=0; j<2; j++) {
				for (int k=0; k<2; k++) {
					if (!face[0][i] || !face[1][j] || !face[2][k]) {
                        point[i][j][k] = renderer->vertex_buffer.index/3;
					    buffer_push(renderer->vertex_buffer, x+i);
                        buffer_push(renderer->vertex_buffer, y+j);
						buffer_push(renderer->vertex_buffer, z+k);
					} 
				}
			}
		}

		int i = 0, j = 0, k = 0;
		int t=0;
		for (int l=0; 1; l++) {
            if (!face[2][k]) {
                buffer_push(renderer->element_buffer, point[i][j][k]);
                buffer_push(renderer->element_buffer, point[1-i][j][k]);
                buffer_push(renderer->element_buffer, point[i][1-j][k]);
            }
            if (!face[1][j]) {
                buffer_push(renderer->element_buffer, point[i][j][1-k]);
                buffer_push(renderer->element_buffer, point[i][j][k]);
                buffer_push(renderer->element_buffer, point[1-i][j][k]);
            }
            if (!face[0][i]) {
                buffer_push(renderer->element_buffer, point[i][1-j ][k]);
                buffer_push(renderer->element_buffer, point[i][j][1-k]);
                buffer_push(renderer->element_buffer, point[i][j][k]);
            }
            if (l==3) break;
            t = ~(1<<l);
            i = 1&t;
            j = (2&t)>>1;
            k = (4&t)>>2;
		}
	}

	log_command("A");
	log_command("2K");
}
