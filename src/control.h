#ifndef CONTROL_H
#define CONTROL_H


#include <math.h>
#include "common.h"
#include "player.h"

enum control_key {
	CONTROL_KEY_FORWARD = 1 << 0,
	CONTROL_KEY_BACKWARD = 1 << 1,
	CONTROL_KEY_LEFT = 1 << 2,
	CONTROL_KEY_RIGHT = 1 << 3,
	CONTROL_KEY_UP = 1 << 4,
	CONTROL_KEY_DOWN = 1 << 5,
};

void control_move(struct player_t *player, u32 keys, f32 delta);


#endif