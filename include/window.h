#ifndef WINDOW_H
#define WINDOW_H

#include "common.h"

#include <SDL2/SDL.h>

typedef struct event_data_t event_data_t;

typedef struct window_t {
	SDL_Window *sdl_window;
	SDL_GLContext *gl_context;
	u32 width, height;
	bool close_requested;

    void (*mouse_button_callback)(struct window_t *, i32, i32, u8);
	void (*mouse_move_callback)(struct window_t *, i32, i32);
	void (*key_callback)(struct window_t *, SDL_Keycode , enum state);
    event_data_t *callback_data;

	f64 time_s;
	u64 time_delta;
	u64 time_ms;
	u64 time_us;
} window_t;

window_t * window_create(u32 width, u32 height);
void window_destroy(window_t *window);

void window_swap(window_t *window);

u64 window_get_time_ms(const window_t *window);
f64 window_get_time_s(const window_t *window);
f64 window_get_time_delta(const window_t *window);


void window_set_mouse_move_callback(
	window_t *window,
	void (*callback)(window_t *, i32, i32)
);

void window_set_key_callback(
	window_t *window,
	void (*callback)(window_t  *, SDL_Keycode, enum state)
);

void window_set_mouse_button_callback(
        window_t *window,
        void (*callback)(window_t *, i32, i32, u8)
);

#endif
