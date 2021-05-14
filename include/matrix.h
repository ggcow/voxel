#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include "core/common.h"
#include "core/opengl.h"

typedef struct {
    GLfloat m[16];
} matrix_t;

#define matrix_identity (matrix_t) {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}

matrix_t matrix_perspective(f32 fov, f32 aspect, f32 near, f32 far);
matrix_t matrix_lookAt(f32 position[3], f32 look[3], f32 up[3]);

matrix_t matrix_multiply(const matrix_t l, const matrix_t r);

matrix_t matrix_rotation_x(const f32 angle);
matrix_t matrix_rotation_y(const f32 angle);
matrix_t matrix_rotation_z(const f32 angle);
matrix_t matrix_translation(const f32 t[3]);



static void _normalize(f32 *x);
static inline f32 _dot(f32 x[3], f32 y[3]);
static void _cross(f32 x[3], f32 y[3], f32 *cross);

static void _matrix_display(matrix_t matrix);

#endif