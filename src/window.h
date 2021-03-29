#ifndef WINDOW_H
#define WINDOW_H

#include "common.h"

#include <SDL2/SDL.h>

enum window_key {
	WINDOW_KEY_ENTER = 0,
	WINDOW_KEY_LEFT,
	WINDOW_KEY_RIGHT,
	WINDOW_KEY_z,
	WINDOW_KEY_q,
	WINDOW_KEY_s,
	WINDOW_KEY_d,
	WINDOW_KEY_SHIFT,
	WINDOW_KEY_SPACE,
};

struct window_t {
	SDL_Window *sdl_window;
	SDL_GLContext *gl_context;
	u32 width, height;
	bool close_requested;

	void (*mouse_move_callback)(void *, i32, i32, u32);
	void *mouse_move_callback_data;

	void (*key_down_callback)(void *, enum window_key, enum state, u32);
	void (*key_up_callback)(void *, enum window_key, enum state, u32);
	void *key_callback_data;
	bool enable_cursor;
	f64 time_s;
	u64 time_delta;
	u64 time_ms;
	u64 time_us;
	u32 sdl_time;
};

struct window_t * window_create(u32 width, u32 height);
void window_destroy(struct window_t *window);

void window_swap(struct window_t *window);
void window_skip_frame(struct window_t *window);

u32 window_get_width(const struct window_t *window);
u32 window_get_height(const struct window_t *window);

void window_enable_cursor(struct window_t *window, bool enable);

u64 window_get_time_ms(const struct window_t *window);
f64 window_get_time_s(const struct window_t *window);
f64 window_get_time_delta(const struct window_t *window);


void window_set_mouse_move_callback(
	struct window_t *window,
	void (*callback)(void *, i32, i32, u32),
	void *data
);

void window_set_key_down_callback(
	struct window_t *window,
	void (*callback)(void *, enum window_key, enum state, u32),
	void *data
);

void window_set_key_up_callback(
	struct window_t *window,
	void (*callback)(void *, enum window_key, enum state, u32),
	void *data
);

#endif
