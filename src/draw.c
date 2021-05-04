#include "../include/draw.h"
#include "../include/opengl.h"

void
draw_cubes(struct renderer_t *renderer, struct map_t *map)
{
	u32 loading = 0;

	fprintf(stderr, "\n");
	for (int index=1; index<map->index; index++) {

		if (10.0f*index/map->index > loading) {
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

		i32 x = map->cubes[index].x;
		i32 y = map->cubes[index].y;
		i32 z = map->cubes[index].z;

		_check_buffer_size(renderer, 8*3, 3*2*6);

		GLfloat *b = renderer->buffer;
		GLuint *v = renderer->indices;
		u32 *index = &(renderer->buffer_index);


		bool face[3][2];
		u32 point[2][2][2];

		for (int i=0; i<2; i++) {
			face[0][i] = map_v(x+(i<<1)-1, y, z)?(map_g(x+(i<<1)-1, y, z) > 0):FALSE;
			face[1][i] = map_v(x, y+(i<<1)-1, z)?(map_g(x, y+(i<<1)-1, z) > 0):FALSE;
			face[2][i] = map_v(x, y, z+(i<<1)-1)?(map_g(x, y, z+(i<<1)-1) > 0):FALSE;
		}


		for (int i=0; i<2; i++) {
			for (int j=0; j<2; j++) {
				for (int k=0; k<2; k++) {
					if (!face[0][i] || !face[1][j] || !face[2][k]) {
                        point[i][j][k] = *index/3;
					    b[(*index)++] = x+i;
					    b[(*index)++] = y+j;
						b[(*index)++] = z+k;
					} 
				}
			}
		}

		index = &(renderer->indices_index);

		for (int i=0; i<2; i++) {
			for (int j=0; j<2; j++) {
				for (int k=0; k<2; k++) {
					if (!(i^(j^k))) {
						if (!face[2][k]) {
							v[(*index)++] = point[i][j][k];
							v[(*index)++] = point[1-i][j][k];
							v[(*index)++] = point[i][1-j][k];
						}
						if (!face[1][j]) {
							v[(*index)++] = point[i][j][k];
							v[(*index)++] = point[1-i][j][k];
							v[(*index)++] = point[i][j][1-k];
						}
						if (!face[0][i]) {
							v[(*index)++] = point[i][1-j ][k];
							v[(*index)++] = point[i][j][1-k];
							v[(*index)++] = point[i][j][k];
						}
					}
				}
			}
		}
	}
	log_command("A");
	log_command("2K");
}


static void
_check_buffer_size(struct renderer_t *renderer, u32 buffer_added_size, u32 indices_added_size)
{
	while (renderer->buffer_index + buffer_added_size >= renderer->buffer_size) {
		renderer->buffer_size *= 2;
		renderer->buffer = reallocate(renderer->buffer, sizeof(GLfloat), renderer->buffer_size);
	}

	while (renderer->indices_index + indices_added_size >= renderer->indices_size) {
		renderer->indices_size *= 2;
		renderer->indices = reallocate(renderer->indices, sizeof(GLuint), renderer->indices_size);
	}
}  