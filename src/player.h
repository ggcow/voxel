#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"

struct  player_t {
	f32 eye[3];

	f32 inclination;
	f32 azimuth;

	f32 look[3];

	f32 speed;
};


struct player_t * player_create(void);
void player_destroy(struct player_t *player);


void player_set_look(struct player_t *player, i32 dx, i32 dy);



#endif