#ifndef CUBE_H
#define CUBE_H

#include "common.h"

enum block {
    BLOCK_NONE,
    BLOCK_DIRT
};

typedef struct cube_t {
	i32 x, y, z;
	enum block type;
} cube_t;

#endif
