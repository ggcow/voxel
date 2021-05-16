#include <math.h>
#include "player.h"

player_t * player_create(map_t *map) {
	player_t *player = callocate(sizeof(player_t));

	player->eye[0]=0.0f;
	player->eye[1]=5.0f;
	player->eye[2]=0.0f;

	player->inclination=0.0f;
	player->azimuth=0.0f;

	player->speed=10.0f;

	player->rendering_distance = 1;

	player->chunk_list = plist_create(map_chunk_get(0, 0, map));

	log_debug("Player created");
	return player;
}

void player_destroy(player_t *player) {
    plist_destroy(player->chunk_list);
	deallocate(player);
	log_debug("Player destroyed");
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