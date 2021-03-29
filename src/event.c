#include "event.h"

void
mouse_move_callback(void *data, i32 dx, i32 dy, u32 time)
{
	struct event_data_t *event_data = (struct event_data_t*)data;
	player_set_look(event_data->player_adress, dx, dy);
}

struct event_data_t *
event_data_create(struct player_t *player)
{
	struct event_data_t *event_data = allocate(sizeof(struct event_data_t), 1);
	event_data->keys=0;
	event_data->player_adress=player;
	log_debug("Event data created");
	return event_data;
}


void
event_data_destroy(struct event_data_t *event_data)
{
	deallocate(event_data);
	log_debug("Event data destroyed");
}

void
key_callback(void *data, enum window_key key, enum state key_state, u32 time)
{
	struct event_data_t *event_data = (struct event_data_t*)data;

	switch (key) {
	case WINDOW_KEY_ENTER:
		*(event_data->close_requested) = TRUE;
		break;

	case WINDOW_KEY_z:
		if(key_state == DOWN) {
			event_data->keys |= CONTROL_KEY_FORWARD;
		} else {
			event_data->keys &= ~CONTROL_KEY_FORWARD;
		}
		break;

	case WINDOW_KEY_q:
		if(key_state == DOWN) {
			event_data->keys |= CONTROL_KEY_LEFT;
		} else {
			event_data->keys &= ~CONTROL_KEY_LEFT;
		}
		break;

	case WINDOW_KEY_s:
		if(key_state == DOWN) {
			event_data->keys |= CONTROL_KEY_BACKWARD;
		} else {
			event_data->keys &= ~CONTROL_KEY_BACKWARD;
		}
		break;

	case WINDOW_KEY_d:
		if(key_state == DOWN) {
			event_data->keys |= CONTROL_KEY_RIGHT;
		} else {
			event_data->keys &= ~CONTROL_KEY_RIGHT;
		}
		break;

	case WINDOW_KEY_SHIFT:
		if(key_state == DOWN) {
			event_data->keys |= CONTROL_KEY_DOWN;
		} else {
			event_data->keys &= ~CONTROL_KEY_DOWN;
		}
		break;

	case WINDOW_KEY_SPACE:
		if(key_state == DOWN) {
			event_data->keys |= CONTROL_KEY_UP;
		} else {
			event_data->keys &= ~CONTROL_KEY_UP;
		}
		break;
	}

}


void window_poll_events(struct window_t *window) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN && window->key_down_callback != NULL) {
			switch(event.key.keysym.sym) {
			case SDLK_RETURN:
				window->key_down_callback(
					window->key_callback_data,
					WINDOW_KEY_ENTER,
					DOWN,
					window->time_ms
				);
				break;

			case SDLK_LEFT:
				window->key_down_callback(
					window->key_callback_data,
					WINDOW_KEY_LEFT,
					DOWN,
					window->time_ms
				);
				break;

			case SDLK_RIGHT:
				window->key_down_callback(
					window->key_callback_data,
					WINDOW_KEY_RIGHT,
					DOWN,
					window->time_ms
				);
				break;

			case SDLK_z:
				window->key_down_callback(
					window->key_callback_data,
					WINDOW_KEY_z,
					DOWN,
					window->time_ms
				);
				break;

			case SDLK_q:
				window->key_down_callback(
					window->key_callback_data,
					WINDOW_KEY_q,
					DOWN,
					window->time_ms
				);
				break;

			case SDLK_s:
				window->key_down_callback(
					window->key_callback_data,
					WINDOW_KEY_s,
					DOWN,
					window->time_ms
				);
				break;

			case SDLK_d:
				window->key_down_callback(
					window->key_callback_data,
					WINDOW_KEY_d,
					DOWN,
					window->time_ms
				);
				break;

			case SDLK_LSHIFT:
				window->key_down_callback(
					window->key_callback_data,
					WINDOW_KEY_SHIFT,
					DOWN,
					window->time_ms
				);
				break;

			case SDLK_SPACE:
				window->key_down_callback(
					window->key_callback_data,
					WINDOW_KEY_SPACE,
					DOWN,
					window->time_ms
				);
				break;
			}

		} else if (event.type == SDL_KEYUP && window->key_up_callback != NULL) {
			switch(event.key.keysym.sym) {
			case SDLK_RETURN:
				window->key_up_callback(
					window->key_callback_data,
					WINDOW_KEY_ENTER,
					UP,
					window->time_ms
				);
				break;

			case SDLK_LEFT:
				window->key_up_callback(
					window->key_callback_data,
					WINDOW_KEY_LEFT,
					UP,
					window->time_ms
				);
				break;

			case SDLK_RIGHT:
				window->key_up_callback(
					window->key_callback_data,
					WINDOW_KEY_RIGHT,
					UP,
					window->time_ms
				);
				break;

			case SDLK_z:
				window->key_up_callback(
					window->key_callback_data,
					WINDOW_KEY_z,
					UP,
					window->time_ms
				);
				break;

			case SDLK_q:
				window->key_up_callback(
					window->key_callback_data,
					WINDOW_KEY_q,
					UP,
					window->time_ms
				);
				break;

			case SDLK_s:
				window->key_up_callback(
					window->key_callback_data,
					WINDOW_KEY_s,
					UP,
					window->time_ms
				);
				break;

			case SDLK_d:
				window->key_up_callback(
					window->key_callback_data,
					WINDOW_KEY_d,
					UP,
					window->time_ms
				);
				break;

			case SDLK_LSHIFT:
				window->key_up_callback(
					window->key_callback_data,
					WINDOW_KEY_SHIFT,
					UP,
					window->time_ms
				);
				break;

			case SDLK_SPACE:
				window->key_up_callback(
					window->key_callback_data,
					WINDOW_KEY_SPACE,
					UP,
					window->time_ms
				);
				break;
			}

		} else if (event.type == SDL_QUIT) {
			window->close_requested = TRUE;
		} else if (window->enable_cursor) {
			if (event.type == SDL_MOUSEBUTTONDOWN) {

			} else if (event.type == SDL_MOUSEMOTION
					&& window->mouse_move_callback != NULL) {
				window->mouse_move_callback(
					window->mouse_move_callback_data,
					event.motion.xrel,
					event.motion.yrel,
					window->time_ms
				);
			} else if (event.type == SDL_MOUSEBUTTONUP) {

			}
		}
	}
}
