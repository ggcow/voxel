#include <math.h>
#include "player.h"
#include "renderer.h"

player_t * player_create(map_t *map) {
	player_t *player = callocate(sizeof(player_t));

	player->eye[0]=CHUNK_SIZE/2;
	player->eye[1]=5.0f;
	player->eye[2]=CHUNK_SIZE/2;

	player->chunk_x = 0;
	player->chunk_z = 0;

	player->inclination=0.0f;
	player->azimuth=0.0f;

	player->speed=10.0f;

	player->rendering_distance = 0;

	player->chunk_list = NULL;
	plist_add(&player->chunk_list, map->chunk00);

	log_debug("Player created");
	return player;
}

void player_destroy(player_t *player) {
    plist_destroy(&player->chunk_list);
	deallocate(player);
	log_debug("Player destroyed");
}

void player_set_chunks(player_t *player, map_t *map) {
    for (int i=-(int)player->rendering_distance-1; i<=(int)player->rendering_distance+1; i++) {
        for (int j=-(int)player->rendering_distance-1; j<=(int)player->rendering_distance+1; j++) {
            chunk_t *chunk = map_chunk_get(i+player->chunk_z, j+player->chunk_x, map);
            if (i*i+j*j<=(int)(player->rendering_distance*player->rendering_distance)) {
                if (!plist_contains(player->chunk_list, chunk)) {
                    plist_add(&player->chunk_list, chunk);
                    chunk_gen_map(chunk);
                }
            } else {
                if (plist_contains(player->chunk_list, chunk)) {
                    plist_remove(&player->chunk_list, chunk);
                }
            }
        }
    }
    renderer_bind_buffers(player);
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