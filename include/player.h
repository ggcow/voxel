#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "chunk.h"
#include "list.h"
#include "map.h"

typedef struct player_t {
	f32 eye[3];
    f32 look[3];

	f32 inclination;
	f32 azimuth;

	f32 speed;
	f32 velocity[3];
	f32 gravity;

	chunk_t *chunk;

    f32 rendering_distance;
	plist_t chunk_list;

    cube_t target;
} player_t;


player_t * player_create(map_t *);
void player_destroy(player_t *);

void player_set_chunks(player_t *, map_t *);
void player_set_look(player_t *, i32 dx, i32 dy);

void player_hit_cube(player_t *, map_t *);
void player_put_cube(player_t *, map_t *);

bool player_collide(f32 eye[3], map_t *);
bool player_collide_cube(f32 eye[3], cube_t);

cube_t player_get_hit_target(player_t *, map_t *);

#endif