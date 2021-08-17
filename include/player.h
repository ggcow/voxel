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

	chunk_t *chunk;

    f32 rendering_distance;
	plist_t chunk_list;
} player_t;


player_t * player_create(map_t *map);
void player_destroy(player_t *player);

void player_set_chunks(player_t *player, map_t *map);
void player_set_look(player_t *player, i32 dx, i32 dy);

void player_reload_chunks(player_t *player);



#endif