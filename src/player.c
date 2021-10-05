#include <math.h>
#include "player.h"

#define PLAYER_RADIUS 0.4

player_t * player_create(map_t *map) {
	player_t *player = callocate(sizeof(player_t));

	player->eye[1]=50.0f;

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
                chunk_free_map(chunk);
                chunk_free_buffer(chunk);
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

	player->look[2]=cosf(*inclination)*cosf(*azimuth);
	player->look[0]=cosf(*inclination)*sinf(*azimuth);
	player->look[1]=sinf(*inclination);
}

cube_t player_get_hit_target(player_t *player, map_t *map) {
    f32 position[] = {player->eye[0], player->eye[1], player->eye[2]};
    int precision = 100;
    for (int i=0; i<4 * precision; i++) {
        for (int j=0; j<3; j++) {
            position[j] += player->look[j] / (float) precision;
        }
        i32 x = (int)floor(position[0]);
        i32 y = (int)floor(position[1]);
        i32 z = (int)floor(position[2]);
        enum block type = map_get_cube(x, y, z, map);
        if (type) {
            return (cube_t) {x, y, z, type};
        }
    }
    return (cube_t) {};
}

static cube_t get_put_target(player_t *player, map_t *map) {
    f32 position[] = {player->eye[0], player->eye[1], player->eye[2]};
    int precision = 100;
    for (int i=0; i<4 * precision; i++) {
        for (int j=0; j<3; j++) {
            position[j] += player->look[j] / (float) precision;
        }
        i32 x = (int)floor(position[0]);
        i32 y = (int)floor(position[1]);
        i32 z = (int)floor(position[2]);
        enum block type = map_get_cube(x, y, z, map);
        if (type) {
            f32 v[] = {position[0]-x-.5f, position[1]-y-.5f, position[2]-z-.5f};
            if (v[0] > v[1] && v[0] > -v[1] && v[0] > v[2] && v[0] > -v[2]) {
                x++;
            } else if (v[0] < v[1] && v[0] < -v[1] && v[0] < v[2] && v[0] < -v[2]) {
                x--;
            } else if (v[1] < v[2] && v[1] < -v[2] && v[1] < v[0] && v[1] < -v[0]) {
                y--;
            } else if (v[1] > v[2] && v[1] > -v[2] && v[1] > v[0] && v[1] > -v[0]) {
                y++;
            } else if (v[2] < v[0] && v[2] < -v[0] && v[2] < v[1] && v[2] < -v[1]) {
                z--;
            } else if (v[2] > v[0] && v[2] > -v[0] && v[2] > v[1] && v[2] > -v[1]) {
                z++;
            }
            cube_t cube = {x, y, z, !map_get_cube(x, y, z, map)};
            if (player_collide_cube(player->eye, cube)) {
                cube.type = BLOCK_NONE;
            }
            return cube;
        }
    }
    return (cube_t) {};
}

void player_hit_cube(player_t *player, map_t *map) {
    if (player->target.type) {
        map_remove_cube(player->target.x, player->target.y, player->target.z, map);
    }
}

void player_put_cube(player_t *player, map_t *map) {
    cube_t cube = get_put_target(player, map);
    if (cube.type) map_add_cube(cube, map);
}

bool player_collide(f32 eye[3], map_t *map)
{
    f32 r = PLAYER_RADIUS;
    i32 y = floor(eye[1]-2.5*r);

    i32 x[2], z[2];


    for (int i=0; i<2; i++) {
        x[i] = (i32) floorf(eye[0] + (i?r:-r));
        z[i] = (i32) floorf(eye[2] + (i?r:-r));
    }

    for (int i=0; i<4; i++) {
        if (map_get_cube(x[i>>1], y, z[i&1], map)) return 1;
    }
    return 0;
}

bool player_collide_cube(f32 eye[3], cube_t cube)
{
    f32 r = PLAYER_RADIUS;
    i32 y = floor(eye[1]-2.5*r);
    i32 x[2], z[2];

    for (int i=0; i<2; i++) {
        x[i] = (i32) floorf(eye[0] + (i?r:-r));
        z[i] = (i32) floorf(eye[2] + (i?r:-r));
    }

    for (int i=0; i<4; i++) {
        if (cube.x == x[i>>1] && cube.y == y && cube.z == z[i&1]) return TRUE;
    }
    return FALSE;
}