#include <stdio.h>

#include "event.h"
#include "window.h"
#include "renderer.h"
#include "player.h"
#include "matrix.h"
#include "cube.h"


#define WIDTH 955
#define HEIGHT 1020

int main(int argc, char *argv[]) {
	log_command("2J");
	log_command("H");

	window_t *window = NULL;
	map_t *map = NULL;
	player_t *player = NULL;
    event_data_t *event_data = NULL;
    renderer_t *renderer = NULL;

	window = window_create(WIDTH, HEIGHT);
	if (!window) {
		log_error("Window could not be created");
		goto exit;
	}

	map = map_create();
	if (!map) {
	    log_error("Map count not be created");
	    goto exit;
	}

	player = player_create(map);
	if (!player) {
		log_error("Player could not be created");
		goto exit;
	}

	event_data = event_data_create(player, map);
	if (!event_data) {
		log_error("Event data could not be created");
		goto exit;
	}

	renderer = renderer_create();
	if (!renderer) {
		log_error("Renderer could not be created");
		goto exit;
	}
	player_set_chunks(player, map);
    control_key_set_defaults();

	window_set_key_callback(window, key_callback, event_data);
	window_set_mouse_move_callback(window, mouse_move_callback, event_data);
	window_set_mouse_button_callback(window, mouse_button_callback, event_data);

	event_data->close_requested = &(window->close_requested);

    matrix_t view, perspective, mvp;
	while (!window->close_requested) {
        event_poll_events(window, &perspective);

		if (control_move(player,
                   map,
                   event_data->keys,
                   (f32)window_get_time_delta(window)/1000000)) {
            player_set_chunks(player, map);
		}

        view = matrix_lookAt(player->eye, player->look, (f32[3]){0,1,0});
        mvp = matrix_multiply(perspective, view);

		renderer_draw(renderer, player, &mvp);
		window_swap(window);
	}


    exit:
	if (renderer) renderer_destroy(renderer);
	if (event_data)	event_data_destroy(event_data);
	if (player)	player_destroy(player);
	if (map) map_destroy(map);
	if (window)	window_destroy(window);
	return 0;
}

