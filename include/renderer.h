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

	GLfloat *buffer;
	u32 buffer_size;
	u32 buffer_index;
	GLuint *indices;
	u32 indices_size;
	u32 indices_index;
};



struct renderer_t * renderer_create(struct map_t *map);

static bool _setup(struct renderer_t *renderer, struct map_t *map);

void renderer_destroy(struct renderer_t *renderer);

void renderer_set_clear_color(struct renderer_t *renderer, f32 r, f32 g, f32 b, f32 a);

void renderer_draw(struct renderer_t *renderer, struct map_t *map, struct player_t *player, u32 width, u32 height);

#endif