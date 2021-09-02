#include <math.h>
#include "player.h"

player_t * player_create(map_t *map) {
	player_t *player = callocate(sizeof(player_t));

	player->eye[0]=CHUNK_SIZE/2;
	player->eye[1]=50.0f;
	player->eye[2]=CHUNK_SIZE/2;

	player->chunk = map->chunk00;

	player->inclination=0.0f;
	player->azimuth=0.0f;

	player->speed=50.0f;

	player->rendering_distance = 2;

	player->chunk_list = NULL;

	log_debug("Player created");
	return player;
}

void player_destroy(player_t *player) {
    plist_destroy(&player->chunk_list);
	deallocate(player);
	log_debug("Player destroyed");
}

void player_set_chunks(player_t *player, map_t *map) {
    i32 r = (int) ceilf(player->rendering_distance)+1;

    for (int i=-3*r; i<=3*r; i++) {
        for (int j =-3*r; j <= 3*r; j++) {
            chunk_t *chunk = map_get_chunk(i + player->chunk->z, j + player->chunk->x, map);
            if (i * i + j * j > 9*r*r-1) {
                chunk_unload(chunk);
            }
            if (i * i + j * j <= (r-1)*(r-1)) {
                if (!plist_contains(player->chunk_list, chunk)) {
                    plist_add(&player->chunk_list, chunk);
                    chunk_gen_map(chunk);
                    chunk_gen_buffer(chunk, map);
                    chunk_load(chunk);
                }
            } else {
                if (plist_contains(player->chunk_list, chunk)) {
                    plist_remove(&player->chunk_list, chunk);
                }
                chunk_free_map(chunk);
                chunk_free_buffer(chunk);
            }
        }
    }
}

void player_set_look(player_t *player, i32 dx, i32 dy) {
	f32 *azimuth = &(player->azimuth);
	f32 *inclination = &(player->inclination);

	*azimuth-=dx/300.0f;
	*inclination-=dy/300.0f;

	if(*inclination>=M_PI_2-0.2f) {
		*inclination=M_PI_2-0.2f;
	}

	if(*inclination<-M_PI_2+0.2f) {
		*inclination=-M_PI_2+0.2f;
	}

	if(*azimuth>=M_PI) {
		*azimuth-=2*M_PI;
	}

	if(*azimuth<-M_PI) {
		*azimuth+=2*M_PI;
	}

	player->look[2]=cos(*inclination)*cos(*azimuth);
	player->look[0]=cos(*inclination)*sin(*azimuth);
	player->look[1]=sin(*inclination);
}

static cube_t * get_target(player_t *player, map_t *map) {
    f32 position[] = {player->eye[0], player->eye[1], player->eye[2]};
    for (int i=0; i<10; i++) {
        for (int j=0; j<3; j++) {
            position[j] += player->look[j];
        }
        if (!map_get_cube(position[0], position[1], position[2], map)) {
            return map_get_cube(position[0], position[1], position[2], map);
        }
    }
    return NULL;
}

void player_hit_cube(player_t *player, map_t *map) {
    cube_t *target = get_target(player, map);
    if (target) {
        map_remove_cube(map, *target);
    }
}

void player_put_cube(player_t *player, map_t *map) {

}