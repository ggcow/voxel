#ifndef RENDERER_H
#define RENDERER_H

#include "core/common.h"
#include "player.h"
#include "event.h"
#include "map.h"

#include "opengl.h"
#include "buffer.h"

typedef struct {
	f32 clear_color[4];
	GLuint program;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	buffer_type(GLint) vertex_buffer;
    buffer_type(GLuint) element_buffer;
} renderer_t;



renderer_t * renderer_create(map_t *map);
static bool _setup(renderer_t *renderer, map_t *map);
void renderer_destroy(renderer_t *renderer);
void renderer_set_clear_color(renderer_t *renderer, f32 r, f32 g, f32 b, f32 a);
void renderer_draw(renderer_t *renderer, map_t *map, player_t *player, u32 width, u32 height);

#endif