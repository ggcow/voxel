#include "core/event.h"

void mouse_move_callback(void *data, i32 dx, i32 dy, u32 time) {
	event_data_t *event_data = (event_data_t*)data;
	player_set_look(event_data->player_adress, dx, dy);
}

event_data_t * event_data_create(player_t *player) {
	event_data_t *event_data = allocate(sizeof(event_data_t), 1);
	event_data->keys=0;
	event_data->player_adress=player;
	log_debug("Event data created");
	return event_data;
}


void event_data_destroy(event_data_t *event_data) {
	deallocate(event_data);
	log_debug("Event data destroyed");
}

void key_callback(void *data, SDL_KeyCode key_code, enum state key_state, u32 time) {
	event_data_t *event_data = (event_data_t*)data;

	enum control_key_t key = control_key_from_sdl_keycode(key_code);
	if (key != CONTROL_KEY_UNKNOWN) {
        if(key_state == DOWN) {
            event_data->keys |= key;
        } else {
            event_data->keys &= ~key;
        }
        return;
	}

	switch (key_code) {
	    case SDLK_ESCAPE:
        case SDLK_RETURN:
            *(event_data->close_requested) = TRUE;
            break;
    }
}


void window_poll_events(window_t *window) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            window->close_requested = TRUE;
        } else if (event.type == SDL_KEYDOWN && window->key_callback) {
            window->key_callback(window->key_callback_data,
                                      event.key.keysym.sym,
                                      DOWN,
                                      window->time_ms);
		} else if (event.type == SDL_KEYUP && window->key_callback) {
            window->key_callback(window->key_callback_data,
                                 event.key.keysym.sym,
                                 UP,
                                 window->time_ms);
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
