#ifndef WINDOW_H
#define WINDOW_H

#include "common.h"

#include <SDL2/SDL.h>

typedef struct window_t {
	SDL_Window *sdl_window;
	SDL_GLContext *gl_context;
	u32 width, height;
	bool close_requested;

	void (*mouse_move_callback)(void *, i32, i32, u32);
	void *mouse_move_callback_data;

	void (*key_callback)(void *, SDL_KeyCode , enum state, u32);
	void *key_callback_data;

    void (*mouse_button_callback)(void *, i32, i32, u8, u32);
    void *mouse_button_callback_data;

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
	void (*callback)(void *, i32, i32, u32),
	void *data
);

void window_set_key_callback(
	window_t *window,
	void (*callback)(void *, SDL_KeyCode, enum state, u32),
	void *data
);

void window_set_mouse_button_callback(
        window_t *window,
        void (*callback)(void *, i32, i32, u8, u32),
        void *data
);

#endif
