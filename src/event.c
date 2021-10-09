#include "event.h"
#include "opengl.h"

void mouse_move_callback(window_t *window, i32 dx, i32 dy) {
	event_data_t *event_data = window->callback_data;
	player_set_look(event_data->player, dx, dy);
}

void mouse_button_callback(window_t *window, i32 dx, i32 dy, u8 button) {
    event_data_t *event_data = window->callback_data;
    switch (button) {
        case SDL_BUTTON_LEFT:
            player_hit_cube(event_data->player, event_data->map);
            break;
        case SDL_BUTTON_RIGHT:
            player_put_cube(event_data->player, event_data->map);
            break;
        default:break;
    }
}

void key_callback(window_t*window, SDL_Keycode key_code, enum state key_state) {
    event_data_t *event_data = window->callback_data;
    enum control_key_t key = control_key_from_sdl_keycode(key_code);
    if (key != KEY_UNKNOWN) {
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
        default:;
    }
}

event_data_t * event_data_create(player_t *player, map_t *map) {
	event_data_t *event_data = callocate(sizeof(event_data_t));
	event_data->keys = 0;
	event_data->player = player;
	event_data->map = map;
	log_debug("Event data created");
	return event_data;
}


void event_data_destroy(event_data_t *event_data) {
	deallocate(event_data);
	log_debug("Event data destroyed");
}


void event_poll_events(window_t *window, matrix_t *perspective) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            window->close_requested = TRUE;
        } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
            SDL_GetWindowSize(
                    window->sdl_window,
                    (i32 *) &window->width,
                    (i32 *) &window->height
            );
            glViewport(0, 0, window->width, window->height);
            *perspective = matrix_perspective(radians(90.0f),
                                              (f32) window->width/window->height,
                                              0.0001f,
                                              1000.0f);
        } else if (event.type == SDL_KEYDOWN && window->key_callback) {
            window->key_callback(
                    window,
                    event.key.keysym.sym,
                    DOWN
            );
        } else if (event.type == SDL_KEYUP && window->key_callback) {
            window->key_callback(
                    window,
                    event.key.keysym.sym,
                    UP
            );
        } else if (event.type == SDL_MOUSEMOTION && window->mouse_move_callback) {
            window->mouse_move_callback(
                window,
                event.motion.xrel,
                event.motion.yrel
            );
        } else if (event.type == SDL_MOUSEBUTTONDOWN && window->mouse_button_callback) {
            window->mouse_button_callback(
                window,
                event.button.x,
                event.button.y,
                event.button.button
            );
        }
	}
}
