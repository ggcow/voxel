#include <stdio.h>

#include "core/event.h"
#include "core/window.h"
#include "core/renderer.h"
#include "player.h"
#include "map.h"
#include "cube.h"


#define WIDTH 955
#define HEIGHT 1020


int main(int argc, char *argv[])
{
	log_command("2J");
	log_command("H");

	window_t *window = NULL;
	player_t *player = NULL;
    event_data_t *event_data = NULL;
    map_t *map = NULL;
    renderer_t *renderer = NULL;

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

	 map = map_create();
	if (map == NULL) {
		log_error("Map could not be created");
		goto exit;
	}

	renderer = renderer_create(map);
	if (renderer == NULL) {
		log_error("Renderer could not be created");
		goto exit;
	}

    control_key_set_defaults();

	log_info("map->map : %zu MiB", map->width * map->length * map->height * sizeof(map->_map[0]) / 1024 / 1024);
    log_info("map->cubes : %zu MiB", map->size * sizeof(map->cubes[0]) / 1024/1024);
    log_info("renderer->buffer : %zu MiB (%d%% used)", renderer->buffer_size * sizeof(renderer->buffer[0]) / 1024/1024,
             (int) (1.0f*renderer->buffer_index*100/renderer->buffer_size));
    log_info("renderer->indices : %zu MiB (%d%% used)", renderer->indices_size * sizeof(renderer->indices[0]) / 1024/1024,
             (int) (1.0f*renderer->indices_index*100/renderer->indices_size));
    fflush(stdout);

	window_set_key_callback(window, key_callback, event_data);
	window_set_mouse_move_callback(window, mouse_move_callback, event_data);
	window_enable_cursor(window, TRUE);

	event_data->close_requested = &(window->close_requested);


	while (!window->close_requested) {
		window_poll_events(window);
		control_move(player, event_data->keys, (f32)window_get_time_delta(window)/1000000);
		renderer_draw(renderer, map, player, WIDTH, HEIGHT);
		window_swap(window);
	}


    exit:
	if (renderer) renderer_destroy(renderer);
	if (map) map_destroy(map);
	if (event_data)	event_data_destroy(event_data);
	if (player)	player_destroy(player);
	if (window)	window_destroy(window);
	return 0;
}

