#ifndef CONTROL_H
#define CONTROL_H


#include <math.h>
#include "common.h"
#include "player.h"

#include <SDL2/SDL_keycode.h>

enum control_key_t {
	CONTROL_KEY_FORWARD = 1 << 0,
	CONTROL_KEY_BACKWARD = 1 << 1,
	CONTROL_KEY_LEFT = 1 << 2,
	CONTROL_KEY_RIGHT = 1 << 3,
	CONTROL_KEY_UP = 1 << 4,
	CONTROL_KEY_DOWN = 1 << 5,
	CONTROL_KEY_UNKNOWN = 1 << 6,
};

#define NUMBER_OF_CONTROL_KEYS __builtin_ctz(CONTROL_KEY_UNKNOWN)

void control_key_set_defaults();
enum control_key_t control_key_from_sdl_keycode(SDL_KeyCode);

void control_move(struct player_t *player, u32 keys, f32 delta);


#endif