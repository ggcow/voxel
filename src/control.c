#include "control.h"
#include "chunk.h"

#define KEY(x) control_key_map[__builtin_ctz(x)]

int control_key_map[NUMBER_OF_CONTROL_KEYS];

void control_key_set_defaults(void) {
    KEY(KEY_BACKWARD) = SDLK_s;
    KEY(KEY_DOWN) = SDLK_LSHIFT;
    KEY(KEY_FORWARD) = SDLK_z;
    KEY(KEY_LEFT) = SDLK_q;
    KEY(KEY_UP) = SDLK_SPACE;
    KEY(KEY_RIGHT) = SDLK_d;
    KEY(KEY_INCREMENT_RENDERING_DISTANCE) = SDLK_p;
    KEY(KEY_DECREMENT_RENDERING_DISTANCE) = SDLK_m;
}

enum control_key_t control_key_from_sdl_keycode(SDL_KeyCode key_code) {
    for (register int i=0; i<NUMBER_OF_CONTROL_KEYS; i++) {
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
 * @param keys
 * @param delta
 * @return TRUE if player changed chunk
 */
bool control_move(player_t *player, u32 keys, f32 delta) {
    bool refresh_chunks = FALSE;
    if (!(keys & KEY_INCREMENT_RENDERING_DISTANCE) != !(keys & KEY_DECREMENT_RENDERING_DISTANCE)) {
        if (keys & KEY_INCREMENT_RENDERING_DISTANCE) {
            player->rendering_distance += 0.1f;
            if (player->rendering_distance > 10) {
                player->rendering_distance = 10;
            }
        } else {
            player->rendering_distance -= 0.1f;
            if (player->rendering_distance < 0) {
                player->rendering_distance = 0;
            }
        }
        log_info("rendering distance : %f", player->rendering_distance);
        refresh_chunks = TRUE;
    }

    i32 last_chunk_x = player->chunk_x;
    i32 last_chunk_z = player->chunk_z;

    f32 direction[2];
    direction[0] = player->look[0];
    direction[1] = player->look[2];

    f32 n = direction[0] * direction[0] + direction[1] * direction[1];

    if (n != 0) {
        n = sqrt(n);
        direction[0] /= n;
        direction[1] /= n;
    }

    if (!(keys & KEY_FORWARD) != !(keys & KEY_BACKWARD)) {
        if (keys & KEY_FORWARD) {
            player->eye[2] += direction[1] * delta * player->speed;
            player->eye[0] += direction[0] * delta * player->speed;
        } else {
            player->eye[2] -= direction[1] * delta * player->speed;
            player->eye[0] -= direction[0] * delta * player->speed;
        }
    }

    if (!(keys & KEY_LEFT) != !(keys & KEY_RIGHT)) {
        if (keys & KEY_LEFT) {
            player->eye[0] += direction[1] * delta * player->speed;
            player->eye[2] -= direction[0] * delta * player->speed;
        } else {
            player->eye[0] -= direction[1] * delta * player->speed;
            player->eye[2] += direction[0] * delta * player->speed;
        }
    }

    if (!(keys & KEY_UP) != !(keys & KEY_DOWN)) {
        if (keys & KEY_DOWN) {
            player->eye[1] -= 0.5f * delta * player->speed;;
        } else {
            player->eye[1] += 0.5f * delta * player->speed;;
        }
    }

    player->chunk_x = floorf(player->eye[0] / CHUNK_SIZE);
    player->chunk_z = floorf(player->eye[2] / CHUNK_SIZE);

    refresh_chunks |= player->chunk_x != last_chunk_x || player->chunk_z != last_chunk_z;

    return refresh_chunks;
}