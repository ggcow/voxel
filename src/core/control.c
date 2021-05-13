#include "core/control.h"

#define KEY(x) control_key_map[__builtin_ctz(x)]

int control_key_map[NUMBER_OF_CONTROL_KEYS];

void control_key_set_defaults() {
    KEY(CONTROL_KEY_BACKWARD) = SDLK_s;
    KEY(CONTROL_KEY_DOWN) = SDLK_LSHIFT;
    KEY(CONTROL_KEY_FORWARD) = SDLK_z;
    KEY(CONTROL_KEY_LEFT) = SDLK_q;
    KEY(CONTROL_KEY_UP) = SDLK_SPACE;
    KEY(CONTROL_KEY_RIGHT) = SDLK_d;
}

enum control_key_t control_key_from_sdl_keycode(SDL_KeyCode key_code) {
    for (register int i=0; i<NUMBER_OF_CONTROL_KEYS; i++) {
        if (control_key_map[i] == key_code) {
            return (enum control_key_t) 1 << i;
        }
    }
    return CONTROL_KEY_UNKNOWN;
}

#undef KEY

void control_move(player_t *player, u32 keys, f32 delta) {
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




