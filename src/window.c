#define _POSIX_C_SOURCE 200809L

#include "window.h"
#include "timer.h"
#include "opengl.h"


int fpscount = 0;
unsigned int fps = 0;

void _update_metrics(struct window_t *window) {
	SDL_GetWindowSize(
		window->sdl_window,
		(i32 *)&window->width,
		(i32 *)&window->height
	);

	window->time_s = timer_time_s();
	window->time_ms = timer_time_ms();
	u64 time = timer_time_us();
	window->time_delta = time - window->time_us;

	if (log_real()) {
		fps += (int)(1000000/(double)window->time_delta);
		fpscount++;
		if (fpscount == 10) {
			log_command("20D");
			info("FPS : %u", fps/fpscount);
			fpscount=0;
			fps=0;
		}
	}

	window->time_us = time;
	window->sdl_time = SDL_GetTicks();	
}

struct window_t *
window_create(u32 width, u32 height)
{
	struct window_t *window = allocate(sizeof(struct window_t), 1);

	if (SDL_WasInit(SDL_INIT_VIDEO) == 0) {
		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
			log_error("SDL initialization failed: %s", SDL_GetError());

			deallocate(window);
			return NULL;
		} else {
			log_debug("SDL initialized");
		}
	}

	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);

	width = (u32)DM.w;
	height = (u32)DM.h;log_error("%u %u", width, height);

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

void window_destroy(struct window_t *window) {
	SDL_GL_DeleteContext(window->gl_context);
	log_debug("Context destroyed");

	SDL_DestroyWindow(window->sdl_window);
	log_debug("Window destroyed");

	SDL_Quit();
	log_debug("SDL uninitialized");

	deallocate(window);
}

void window_swap(struct window_t *window) {
	SDL_GL_SwapWindow(window->sdl_window);
	_update_metrics(window);
}

void window_skip_frame(struct window_t *window) {
	SDL_WaitEvent(NULL);
	_update_metrics(window);
}

u32
window_get_width(const struct window_t *window)
{
	return window->width;
}

u32
window_get_height(const struct window_t *window)
{
	return window->height;
}


f64 window_get_time_delta(const struct window_t *window)
{
	return window->time_delta;
}

u64
window_get_time_ms(const struct window_t *window)
{
	return window->time_ms;
}

f64
window_get_time_s(const struct window_t *window)
{
	return window->time_s;
}


void window_set_mouse_move_callback(struct window_t *window,
	void (*callback)(void *, i32, i32, u32),
	void *data)
{
	window->mouse_move_callback = callback;
	window->mouse_move_callback_data = data;
}


void window_set_key_down_callback(struct window_t *window,
	void (*callback)(void *, enum window_key, enum state, u32),
	void *data)
{
	window->key_down_callback = callback;
	window->key_callback_data = data;
}

void window_set_key_up_callback(struct window_t *window,
	void (*callback)(void *, enum window_key, enum state, u32),
	void *data)
{
	window->key_up_callback = callback;
	window->key_callback_data = data;
}

void
window_enable_cursor(struct window_t *window, bool enable)
{
	window->enable_cursor = enable;
}