#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include "common.h"

#define TEXTURE_SIZE 16

enum texture_t {
    TEXTURE_DIRT,
    TEXTURE_TOP,
    TEXTURE_SIDE,
    TEXTURE_COUNT
};

void texture_setup(void);

#endif //OPENGL_TEXTURE_H
