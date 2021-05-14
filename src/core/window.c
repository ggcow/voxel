#include "core/window.h"
#include "core/timer.h"
#include "core/opengl.h"

static void _update_metrics(window_t *window) {
    SDL_GetWindowSize(
            window->sdl_window,
            (i32 *)&window->width,
            (i32 *)&window->height
    );

    static u32 frame_count = 0;
    static f64 frame_start_time = 0;

    window->time_ms = timer_time_ms();
    u64 time = timer_time_us();
    window->time_delta = time - window->time_us;
    frame_count++;
    if (timer_time_s() - frame_start_time > 1) {
        info("FPS : %u\n", frame_count);
        log_command("A");
        frame_count = 0;
        frame_start_time = timer_time_s();
    }

    window->time_us = time;
    window->sdl_time = SDL_GetTicks();
}

window_t * window_create(u32 width, u32 height) {
	window_t *window = allocate(sizeof(window_t));

	if (SDL_WasInit(SDL_INIT_VIDEO) == 0) {
		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
			log_error("SDL initialization failed: %s", SDL_GetError());

			deallocate(window);
			return NULL;
		} else {
			log_debug("SDL initialized");
		}
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	window->sdl_window = SDL_CreateWindow(
		"OpenGL",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	if (window->sdl_window == NULL) {
		log_error("Window creation failed: %s", SDL_GetError());

		deallocate(window);
		return NULL;
	} else {
		log_debug("Window created");
	}

	if (SDL_SetRelativeMouseMode(1)) {
		log_error("Couldn't use SDL_CaptureMouse");
	}

	window->gl_context = SDL_GL_CreateContext(window->sdl_window);
	if (window->gl_context == NULL) {
		log_error("OpenGL context creation failed: %s", SDL_GetError());

		deallocate(window);
		return NULL;
	} else {
		log_debug("OpenGL context created: %s", glGetString(GL_VERSION));
	}

	_update_metrics(window);

	window->width = width;
	window->height = height;

	SDL_GL_SetSwapInterval(0);

	return window;
}

void window_destroy(window_t *window) {
	SDL_GL_DeleteContext(window->gl_context);
	log_debug("Context destroyed");

	SDL_DestroyWindow(window->sdl_window);
	log_debug("Window destroyed");

	SDL_Quit();
	log_debug("SDL uninitialized");

	deallocate(window);
}

void window_swap(window_t *window) {
	SDL_GL_SwapWindow(window->sdl_window);
	_update_metrics(window);
}

void window_skip_frame(window_t *window) {
	SDL_WaitEvent(NULL);
	_update_metrics(window);
}

u32 window_get_width(const window_t *window) {
	return window->width;
}

u32 window_get_height(const window_t *window) {
	return window->height;
}


f64 window_get_time_delta(const window_t *window)
{
	return window->time_delta;
}

u64 window_get_time_ms(const window_t *window) {
	return window->time_ms;
}

f64 window_get_time_s(const window_t *window) {
	return window->time_s;
}


void window_set_mouse_move_callback(window_t *window,
	void (*callback)(void *, i32, i32, u32),
	void *data) {
	window->mouse_move_callback = callback;
	window->mouse_move_callback_data = data;
}


void window_set_key_callback(window_t *window,
	void (*callback)(void *, SDL_KeyCode , enum state, u32),
	void *data) {
	window->key_callback = callback;
	window->key_callback_data = data;
}

void window_enable_cursor(window_t *window, bool enable) {
	window->enable_cursor = enable;
}
