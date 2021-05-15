#include <stdio.h>

#include "event.h"
#include "window.h"
#include "renderer.h"
#include "player.h"
#include "matrix.h"


#define WIDTH 955
#define HEIGHT 1020

int main(int argc, char *argv[])
{
	log_command("2J");
	log_command("H");

	window_t *window = NULL;
	player_t *player = NULL;
    event_data_t *event_data = NULL;
    renderer_t *renderer = NULL;

    pbuffer_t chunk_buffer = pbuffer_make((void (*)(void *)) chunk_destroy);

	window = window_create(WIDTH, HEIGHT);
	if (window == NULL) {
		log_error("Window could not be created");
		goto exit;
	}

	player = player_create();
	if (player == NULL) {
		log_error("Player could not be created");
		goto exit;
	}

	event_data = event_data_create(player);
	if (event_data == NULL) {
		log_error("Event data could not be created");
		goto exit;
	}

	buffer_push(chunk_buffer, chunk_create(0, 0));
	chunk_gen_map(chunk_buffer.data[0]);
	chunk_gen_buffer(chunk_buffer.data[0]);

	renderer = renderer_create(chunk_buffer.data[0]);
	if (renderer == NULL) {
		log_error("Renderer could not be created");
		goto exit;
	}

    control_key_set_defaults();

	window_set_key_callback(window, key_callback, event_data);
	window_set_mouse_move_callback(window, mouse_move_callback, event_data);

	event_data->close_requested = &(window->close_requested);

    matrix_t view, perspective, mvp;

	while (!window->close_requested) {
        event_poll_events(window, &perspective);

		int directions = control_move(player,
                                event_data->keys,
                                (f32)window_get_time_delta(window)/1000000);

		//TODO use directions for chunk check

        view = matrix_lookAt(player->eye, player->look, (f32[3]){0,1,0});
        mvp = matrix_multiply(perspective, view);

		renderer_draw(renderer, player, &mvp, chunk_buffer.data[0]);
		window_swap(window);
	}


    exit:
	if (renderer) renderer_destroy(renderer);
	if (event_data)	event_data_destroy(event_data);
	if (player)	player_destroy(player);
	if (window)	window_destroy(window);
    pbuffer_terminate(chunk_buffer);
	return 0;
}

