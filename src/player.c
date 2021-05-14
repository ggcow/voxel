#include <math.h>
#include "player.h"
#include "matrix.h"

player_t *
player_create(void)
{
	player_t *player = allocate(sizeof(player_t));

	player->eye[0]=0.0f;
	player->eye[1]=5.0f;
	player->eye[2]=0.0f;

	player->inclination=0.0f;
	player->azimuth=0.0f;

	player->speed=1.0f;

	log_debug("Player created");
	return player;
}

void
player_destroy(player_t *player)
{
	deallocate(player);
	log_debug("Player destroyed");
}


void
player_set_look(player_t *player, i32 dx, i32 dy)
{
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

	// log_debug("delta : %d - %d", dx, dy);
	// log_debug("angles : %f - %f", *inclination, *azimuth);
	// log_debug("LOOK\n%f\n%f\n%f\n", player->look[0], player->look[1], player->look[2]);

	player->look[2]=cos(*inclination)*cos(*azimuth);
	player->look[0]=cos(*inclination)*sin(*azimuth);
	player->look[1]=sin(*inclination);
}