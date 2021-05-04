#ifndef EVENT_H
#define EVENT_H

#include "common.h"
#include "window.h"
#include "control.h"
#include "player.h"

struct event_data_t {
	bool *close_requested;
	u32 keys;

	struct player_t *player_adress;
};

void mouse_move_callback(void *data, i32 dx, i32 dy, u32 time);
void key_callback(void *data, SDL_KeyCode key_code, enum state key_state, u32 time);
void window_poll_events(struct window_t *window);

struct event_data_t * event_data_create(struct player_t *player);
void event_data_destroy(struct event_data_t *event_data);

#endif
