#ifndef CONTROL_H
#define CONTROL_H


#include <math.h>
#include "common.h"
#include "player.h"

#if __has_include(<SDL2/SDL_keycode.h>)
    #include <SDL2/SDL_keycode.h>
#else
    #include <SDL2/SDL_KeyCode.h>
#endif

enum control_key_t {
	KEY_FORWARD = 1 << 0,
	KEY_BACKWARD = 1 << 1,
	KEY_LEFT = 1 << 2,
	KEY_RIGHT = 1 << 3,
	KEY_UP = 1 << 4,
	KEY_DOWN = 1 << 5,

    KEY_RENDERING_DISTANCE_PLUS = 1 << 6,
    KEY_RENDERING_DISTANCE_MINUS = 1 << 7,

	KEY_UNKNOWN = 1 << 8
};

#define NUMBER_OF_CONTROL_KEYS __builtin_ctz(KEY_UNKNOWN)

void control_key_set_defaults(void);
enum control_key_t control_key_from_sdl_keycode(SDL_Keycode code);

bool control_move(player_t *player, map_t *map, u32 keys, f32 delta);


#endif