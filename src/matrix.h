#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include "common.h"


void _matrix_identity(f32 *out);

void _matrix_perspective(f32 fovy, f32 aspect, f32 near, f32 far, f32 *matrix);

void _matrix_multiply(const f32 *left, const f32 *right, f32 *result);

void _matrix_rotation_x(const f32 angle, f32 *matrix);
void _matrix_rotation_y(const f32 angle, f32 *matrix);
void _matrix_rotation_z(const f32 angle, f32 *matrix);

void _matrix_translation(const f32 *t, f32 *matrix);

void _matrix_lookAt(f32 *position, f32 *center, f32 *up, f32 *out);

static f32 _abs(f32 x);

static void _normalize(f32 *x);

static f32 _dot(f32 *x, f32 *y);

static void _cross(f32 *x, f32 *y, f32 *cross);

void _matrix_display(f32 *m);

#endif