#ifndef EVENT_H
#define EVENT_H

#include "common.h"
#include "window.h"
#include "control.h"
#include "player.h"
#include "matrix.h"

struct event_data_t {
	bool *close_requested;
	u32 keys;

	player_t *player;
	map_t *map;
};

void mouse_move_callback(window_t *window, i32 dx, i32 dy);
void key_callback(window_t *window, SDL_Keycode key_code, enum state key_state);
void mouse_button_callback(window_t *window, i32 dx, i32 dy, u8 button);

void event_poll_events(window_t *window, matrix_t *perspective);

event_data_t * event_data_create(player_t *player, map_t *map);
void event_data_destroy(event_data_t *event_data);

#endif
