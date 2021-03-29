#include <stdio.h>

#include "event.h"
#include "window.h"
#include "renderer.h"
#include "player.h"
#include "map.h"
#include "cube.h"


#define WIDTH 1920
#define HEIGHT 1080   

 
int main(int argc, char *argv[])
{
	log_command("?25l");
	log_command("2J");
	log_command("H");

	struct window_t *window = window_create(WIDTH, HEIGHT); 

	if (window == NULL) {
		log_error("Window could not be created");
		goto exit_window;
	}

	struct player_t *player = player_create();

	if (player == NULL) {
		log_error("Player could not be created");
		goto exit_player;
	}

	struct event_data_t *event_data = event_data_create(player);

	if (event_data == NULL) {
		log_error("Event data could not be created");
		goto exit_event_data;
	}

	struct map_t *map = map_create();

	if (map == NULL) {
		log_error("Map could not be created");
		goto exit_map;
	}

	struct renderer_t *renderer = renderer_create(map);

	if (renderer == NULL) {
		log_error("Renderer could not be created");
		goto exit_renderer;
	}

	window_set_key_down_callback(window, key_callback, event_data);
	window_set_key_up_callback(window, key_callback, event_data);
	window_set_mouse_move_callback(window, mouse_move_callback, event_data);
	window_enable_cursor(window, TRUE);

	event_data->close_requested = &(window->close_requested);


	while (!window->close_requested) {
		window_poll_events(window);
		control_move(player, event_data->keys, (f32)window_get_time_delta(window)/1000000);
		renderer_draw(renderer, map, player, window, WIDTH, HEIGHT);
		window_swap(window);
	}


	renderer_destroy(renderer);
	exit_renderer:
	map_destroy(map);
	exit_map:
	event_data_destroy(event_data);
	exit_event_data:
	player_destroy(player);
	exit_player:
	window_destroy(window);
	exit_window:
	return 0;
}

