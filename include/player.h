#ifndef PLAYER_H
#define PLAYER_H

#include "core/common.h"

typedef struct {
	f32 eye[3];

	f32 inclination;
	f32 azimuth;

	f32 look[3];

	f32 speed;
} player_t;


player_t * player_create(void);
void player_destroy(player_t *player);


void player_set_look(player_t *player, i32 dx, i32 dy);



#endif