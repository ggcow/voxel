#include "draw.h"
#include "opengl.h"
#include <inttypes.h>
#include <pthread.h>
#include <unistd.h>

#define N_THREADS 4

static void* draw_cubes_thread(void* data_p);

void
draw_cubes(struct renderer_t *renderer, struct map_t *map)
{
	pthread_t thread[N_THREADS];

	struct draw_thread_data_t *thread_data = renderer->draw_thread_data;

	
    for (int i=0; i<N_THREADS; i++) {

       	thread_data[i].indices_index = 0;
 
       	thread_data[i].buffer_index = 0;
		
		thread_data[0].start = 1;
		thread_data[i].end = (i+1)*map->index/N_THREADS;

		if (i>0) {
			thread_data[i].start = thread_data[i-1].end;
		}



    	i32 err = pthread_create(&(thread[i]), NULL, draw_cubes_thread, &(thread_data[i]));
    	
	    if (err) {
	       log_error("An error occured while creating a thread: %d", err);
	       exit(1);
	    }  
	}

	for (int i=0; i<N_THREADS; i++) {
		pthread_join(thread[i], NULL);
	}


	renderer_empty_buffer(renderer);

	for(int i=0; i<N_THREADS; i++) {

		u64 tmp = renderer->buffer_index/3;
		
		for(u64 j = 0; j<thread_data[i].buffer_index; j++) {
			renderer->buffer[renderer->buffer_index] = thread_data[i].buffer[j];
			renderer->buffer_index++;
		}

		for(u64 j = 0; j<thread_data[i].indices_index; j++) {
			renderer->indices[renderer->indices_index] = thread_data[i].indices[j]+tmp;
			renderer->indices_index++;
		}

		//deallocate(thread_data[i].buffer);
		//deallocate(thread_data[i].indices);
	}

	//deallocate(thread_data);

	
}


static void*
draw_cubes_thread(void *data_p)
{
	struct draw_thread_data_t *data = (struct draw_thread_data_t*)data_p;
	struct map_t *map = data->map;

	for (u64 c=data->start; c<data->end; c++) {


		while (data->buffer_index + 8*3 >= data->buffer_size) {
		
			data->buffer_size *= 2;
			data->buffer = reallocate(data->buffer, sizeof(GLint), data->buffer_size);
		}

		while (data->indices_index + 3*2*6 >= data->indices_size) {
			data->indices_size *= 2;
			data->indices = reallocate(data->indices, sizeof(GLuint), data->indices_size);
		}
		
		i32 x = map->cubes[c].x;
		i32 y = map->cubes[c].y;
		i32 z = map->cubes[c].z;

		GLint *b = data->buffer;
		GLuint *v = data->indices;
		u64 *index = &(data->buffer_index);

		bool face[3][2];
		u32 point[2][2][2];

		for (int i=0; i<2; i++) {
			face[0][i] = map_v(x+(i<<1)-1, y, z)?(map->m[offset(x+(i<<1)-1, y, z)] > 0):FALSE;
			face[1][i] = map_v(x, y+(i<<1)-1, z)?(map->m[offset(x, y+(i<<1)-1, z)] > 0):FALSE;
			face[2][i] = map_v(x, y, z+(i<<1)-1)?(map->m[offset(x, y, z+(i<<1)-1)] > 0):FALSE;
		}
 
		for (int i=0; i<2; i++) {
			for (int j=0; j<2; j++) {
				for (int k=0; k<2; k++) {
					if (!face[0][i] || !face[1][j] || !face[2][k]) {
						b[*index]   = x+i;
						b[*index+1] = y+j;
						b[*index+2] = z+k;
						point[i][j][k] = *index/3;
						*index+=3;
					} 
				}
			}
		}

		index = &(data->indices_index);

		for (int i=0; i<2; i++) {
			for (int j=0; j<2; j++) {
				for (int k=0; k<2; k++) {
					if (!(i^(j^k))) {
						if (!face[2][k]) {
							v[*index] = point[i][j][k];
							v[*index+1] = point[1-i][j][k];
							v[*index+2] = point[i][1-j][k];
							*index+=3;
						}
						if (!face[1][j]) {
							v[*index] = point[i][j][k];
							v[*index+1] = point[1-i][j][k];
							v[*index+2] = point[i][j][1-k];
							*index+=3;
						}
						if (!face[0][i]) {
							v[*index] = point[i][1-j ][k];
							v[*index+1] = point[i][j][1-k];
							v[*index+2] = point[i][j][k];
							*index+=3;
						}
					}
				}
			}
		}
	}

	return NULL;
}