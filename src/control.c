#include "control.h"
#include "chunk.h"

#define KEY(x) control_key_map[__builtin_ctz(x)]

int control_key_map[NUMBER_OF_CONTROL_KEYS];

static void collision_check(player_t *player, f32 eye[3], const f32 vel[3], map_t *map);

void control_key_set_defaults(void) {
    KEY(KEY_BACKWARD) = SDLK_s;
    KEY(KEY_DOWN) = SDLK_LSHIFT;
    KEY(KEY_FORWARD) = SDLK_z;
    KEY(KEY_LEFT) = SDLK_q;
    KEY(KEY_UP) = SDLK_SPACE;
    KEY(KEY_RIGHT) = SDLK_d;
    KEY(KEY_RENDERING_DISTANCE_PLUS) = SDLK_p;
    KEY(KEY_RENDERING_DISTANCE_MINUS) = SDLK_m;
}

enum control_key_t control_key_from_sdl_keycode(SDL_KeyCode key_code) {
    for (int i=0; i<NUMBER_OF_CONTROL_KEYS; i++) {
        if (control_key_map[i] == key_code) {
            return (enum control_key_t) 1 << i;
        }
    }
    return KEY_UNKNOWN;
}

#undef KEY

/**
 * Updates the player's look and direction vectors
 * @param player
 * @param map
 * @param keys
 * @param delta
 * @return TRUE if player changed chunk
 */
bool control_move(player_t *player, map_t *map, u32 keys, f32 delta) {
    static u32 last_keys = 0;
    static chunk_t *last_chunk = NULL;

    bool refresh_chunks = FALSE;
    if ((keys & KEY_RENDERING_DISTANCE_PLUS) ^ (keys & KEY_RENDERING_DISTANCE_MINUS)) {
        if (keys & KEY_RENDERING_DISTANCE_PLUS) {
            player->rendering_distance += 0.1f;
            if (player->rendering_distance > 100) {
                player->rendering_distance = 100;
            }
        } else {
            player->rendering_distance -= 0.1f;
            if (player->rendering_distance < 0) {
                player->rendering_distance = 0;
            }
        }
        refresh_chunks = TRUE;
    }

    f32 direction[2];
    direction[0] = player->look[0];
    direction[1] = player->look[2];

    f32 n = direction[0] * direction[0] + direction[1] * direction[1];

    if (n != 0) {
        n = sqrt(n);
        direction[0] /= n;
        direction[1] /= n;
    }

    f32 eye[] = {player->eye[0], player->eye[1], player->eye[2]};
    f32 vel[] = {player->velocity[0], player->velocity[1], player->velocity[2]};

    if (!(keys & KEY_FORWARD) != !(keys & KEY_BACKWARD)) {
        if (keys & KEY_FORWARD) {
            vel[2] += direction[1] * delta;
            vel[0] += direction[0] * delta;
        } else {
            vel[2] -= direction[1] * delta;
            vel[0] -= direction[0] * delta;
        }
    }

    if (!(keys & KEY_LEFT) != !(keys & KEY_RIGHT)) {
        if (keys & KEY_LEFT) {
            vel[0] += direction[1] * delta;
            vel[2] -= direction[0] * delta;
        } else {
            vel[0] -= direction[1] * delta;
            vel[2] += direction[0] * delta;
        }
    }

    if (!(keys & KEY_UP) != !(keys & KEY_DOWN)) {
        if (keys & KEY_DOWN) {
//            vel[1] -= 11 * delta;
        } else {
            vel[1] += 11 * delta;
        }
    }

    vel[1] -= delta * player->gravity;

    for (int i=0; i<3; i++) {
        eye[i] += vel[i] * delta * player->speed;
    }


    collision_check(player, eye, vel, map);

    for (int i=0; i<3; i++) {
        player->velocity[i] *= pow(0.9, delta * 100);
    }

    /* Only reload chunks when key released
    refresh_chunks |= ((last_keys & KEY_RENDERING_DISTANCE_PLUS) && !(keys & KEY_RENDERING_DISTANCE_PLUS))
            || ((last_keys & KEY_RENDERING_DISTANCE_MINUS) && !(keys & KEY_RENDERING_DISTANCE_MINUS));
    */

    player->chunk = map_get_chunk(floorf(player->eye[2] / CHUNK_SIZE),
                                  floorf(player->eye[0] / CHUNK_SIZE),
                                  map);

    refresh_chunks |= player->chunk != last_chunk;

    last_keys = keys;
    last_chunk = player->chunk;
    return refresh_chunks;
}

static void collision_check(player_t *player, f32 eye[3], const f32 vel[3], map_t *map)
{
    int index[] = {0, 1, 2, 4, 3, 6, 5};
    for (int j=0; j<7; j++) {
        int i = index[j];
        f32 eye_check[] = {((1&i)?player->eye:eye)[0], ((2&i)?player->eye:eye)[1], ((4&i)?player->eye:eye)[2]};
        if (!player_collide(eye_check, map)) {
            memcpy(player->eye, eye_check, 3 * sizeof(*player->eye));
            player->velocity[0] = ((1&i)?player->velocity:vel)[0];
            player->velocity[1] = ((2&i)?player->velocity:vel)[1];
            player->velocity[2] = ((4&i)?player->velocity:vel)[2];
            player->gravity = !(i & 2) * 6;
            break;
        }
    }
}
