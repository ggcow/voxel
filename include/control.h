#ifndef CONTROL_H
#define CONTROL_H


#include <math.h>
#include "common.h"
#include "player.h"

#include <SDL2/SDL_keycode.h>

enum control_key_t {
	KEY_FORWARD = 1 << 0,
	KEY_BACKWARD = 1 << 1,
	KEY_LEFT = 1 << 2,
	KEY_RIGHT = 1 << 3,
	KEY_UP = 1 << 4,
	KEY_DOWN = 1 << 5,

    KEY_INCREMENT_RENDERING_DISTANCE = 1 << 6,
    KEY_DECREMENT_RENDERING_DISTANCE = 1 << 7,

	KEY_UNKNOWN = 1 << 8
};

#define NUMBER_OF_CONTROL_KEYS __builtin_ctz(KEY_UNKNOWN)

void control_key_set_defaults(void);
enum control_key_t control_key_from_sdl_keycode(SDL_KeyCode);

bool control_move(player_t *player, u32 keys, f32 delta);


#endif