#ifndef EVENT_H
#define EVENT_H

#include "core/common.h"
#include "window.h"
#include "core/control.h"
#include "player.h"

typedef struct {
	bool *close_requested;
	u32 keys;

	player_t *player_adress;
} event_data_t;

void mouse_move_callback(void *data, i32 dx, i32 dy, u32 time);
void key_callback(void *data, SDL_KeyCode key_code, enum state key_state, u32 time);
void window_poll_events(window_t *window);

event_data_t * event_data_create(player_t *player);
void event_data_destroy(event_data_t *event_data);

#endif
