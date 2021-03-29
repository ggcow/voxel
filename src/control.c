#include "control.h"


void
control_move(struct player_t *player, u32 keys, f32 delta)
{
	f32 direction[2];
	direction[0]=player->look[0];
	direction[1]=player->look[2];

	f32 n = direction[0]*direction[0]+direction[1]*direction[1];

	//log_debug("Direction : \n%f\n%f\n", direction[0], direction[1]);

	if(n != 0) {
		n=sqrt(n);
		direction[0]/=n;
		direction[1]/=n;
	}

	if(!(keys & CONTROL_KEY_FORWARD) != !(keys & CONTROL_KEY_BACKWARD)) {
		if(keys & CONTROL_KEY_FORWARD) {
			player->eye[2]+=direction[1]*delta*player->speed;
			player->eye[0]+=direction[0]*delta*player->speed;
		} else {
			player->eye[2]-=direction[1]*delta*player->speed;
			player->eye[0]-=direction[0]*delta*player->speed;
		}
	}

	if(!(keys & CONTROL_KEY_LEFT) != !(keys & CONTROL_KEY_RIGHT)) {
		if(keys & CONTROL_KEY_LEFT) {
			player->eye[0]+=direction[1]*delta*player->speed;
			player->eye[2]-=direction[0]*delta*player->speed;
		} else {
			player->eye[0]-=direction[1]*delta*player->speed;
			player->eye[2]+=direction[0]*delta*player->speed;
		}
	}

	if(!(keys & CONTROL_KEY_UP) != !(keys & CONTROL_KEY_DOWN)) {
		if(keys & CONTROL_KEY_DOWN) {
			player->eye[1]-=0.5f*delta*player->speed;;
		} else {
			player->eye[1]+=0.5f*delta*player->speed;;
		}
	}
};




