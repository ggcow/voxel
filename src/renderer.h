#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"
#include "player.h"
#include "event.h"
#include "map.h"


#include "opengl.h"


struct renderer_t {
	f32 clear_color[4];
	GLuint program;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	GLint *buffer;
	u64 buffer_size;
	u64 buffer_index;
	GLuint *indices;
	u64 indices_size;
	u64 indices_index;

	struct draw_thread_data_t *draw_thread_data;
	pthread_mutex_t *working_mutex;
	pthread_mutex_t *progress_mutex;
};



struct renderer_t * renderer_create(struct map_t *map);



void renderer_destroy(struct renderer_t *renderer);
void renderer_empty_buffer(struct renderer_t *renderer);
void renderer_set_clear_color(struct renderer_t *renderer, f32 r, f32 g, f32 b, f32 a);
void renderer_draw(struct renderer_t *renderer, struct map_t *map, struct player_t *player, struct window_t *window, u32 width, u32 height);

#endif