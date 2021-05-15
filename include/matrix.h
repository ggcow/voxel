#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include "common.h"
#include "opengl.h"

#define radians(d) ((d) * M_PI / 180.0)

typedef struct matrix_t {
    GLfloat m[16];
} matrix_t;


#define matrix_identity (matrix_t) {{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}}

matrix_t matrix_perspective(f32 fov, f32 aspect, f32 near, f32 far);
matrix_t matrix_lookAt(f32 position[3], f32 look[3], f32 up[3]);

matrix_t matrix_multiply(matrix_t l, matrix_t r);

matrix_t matrix_rotation_x(f32 angle);
matrix_t matrix_rotation_y(f32 angle);
matrix_t matrix_rotation_z(f32 angle);
matrix_t matrix_translation(const f32 t[3]);

#endif