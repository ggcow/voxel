#include "matrix.h"

void
_matrix_identity(f32 *out) {
	out[0] = 1.0f;
	out[1] = 0.0f;
	out[2] = 0.0f;
	out[3] = 0.0f;
	out[4] = 0.0f;
	out[5] = 1.0f;
	out[6] = 0.0f;
	out[7] = 0.0f;
	out[8] = 0.0f;
	out[9] = 0.0f;
	out[10] = 1.0f;
	out[11] = 0.0f;
	out[12] = 0.0f;
	out[13] = 0.0f;
	out[14] = 0.0f;
	out[15] = 1.0f;
}

void
_matrix_perspective(f32 fovy, f32 aspect, f32 near, f32 far, f32 *matrix)
{
	_matrix_identity(matrix);
    f32 tan_half_angle = tan(fovy / 2);
    matrix[0] = 1.0f / (aspect * tan_half_angle);
    matrix[5] = 1 / (tan_half_angle);
    matrix[10] = -(far + near) / (far - near);
    matrix[11] = -1;
    matrix[14] = (2 * far * near) / (near - far);
    matrix[15] = 0.0f;
}

void
_matrix_multiply(const f32 *left, const f32 *right, f32 *result)
{
	result[ 0] = left[0] * right[ 0] + left[4] * right[ 1]
		+ left[ 8] * right[ 2] + left[12] * right[ 3];
	result[ 1] = left[1] * right[ 0] + left[5] * right[ 1]
		+ left[ 9] * right[ 2] + left[13] * right[ 3];
	result[ 2] = left[2] * right[ 0] + left[6] * right[ 1]
		+ left[10] * right[ 2] + left[14] * right[ 3];
	result[ 3] = left[3] * right[ 0] + left[7] * right[ 1]
		+ left[11] * right[ 2] + left[15] * right[ 3];

	result[ 4] = left[0] * right[ 4] + left[4] * right[ 5]
		+ left[ 8] * right[ 6] + left[12] * right[ 7];
	result[ 5] = left[1] * right[ 4] + left[5] * right[ 5]
		+ left[ 9] * right[ 6] + left[13] * right[ 7];
	result[ 6] = left[2] * right[ 4] + left[6] * right[ 5]
		+ left[10] * right[ 6] + left[14] * right[ 7];
	result[ 7] = left[3] * right[ 4] + left[7] * right[ 5]
		+ left[11] * right[ 6] + left[15] * right[ 7];

	result[ 8] = left[0] * right[ 8] + left[4] * right[ 9]
		+ left[ 8] * right[10] + left[12] * right[11];
	result[ 9] = left[1] * right[ 8] + left[5] * right[ 9]
		+ left[ 9] * right[10] + left[13] * right[11];
	result[10] = left[2] * right[ 8] + left[6] * right[ 9]
		+ left[10] * right[10] + left[14] * right[11];
	result[11] = left[3] * right[ 8] + left[7] * right[ 9]
		+ left[11] * right[10] + left[15] * right[11];

	result[12] = left[0] * right[12] + left[4] * right[13]
		+ left[ 8] * right[14] + left[12] * right[15];
	result[13] = left[1] * right[12] + left[5] * right[13]
		+ left[ 9] * right[14] + left[13] * right[15];
	result[14] = left[2] * right[12] + left[6] * right[13]
		+ left[10] * right[14] + left[14] * right[15];
	result[15] = left[3] * right[12] + left[7] * right[13]
		+ left[11] * right[14] + left[15] * right[15];
}

void
_matrix_rotation_x(const f32 angle, f32 *matrix)
{
	f32 c = cos(angle);
	f32 s = sin(angle);

	_matrix_identity(matrix);
	matrix[5]=c;
	matrix[6]=-s;
	matrix[9]=s;
	matrix[10]=c;
}

void
_matrix_rotation_y(const f32 angle, f32 *matrix)
{
	f32 c = cos(angle);
	f32 s = sin(angle);

	_matrix_identity(matrix);
	matrix[0]=c;
	matrix[2]=s;
	matrix[8]=-s;
	matrix[10]=c;
}


void
_matrix_rotation_z(const f32 angle, f32 *matrix)
{
	f32 c = cos(angle);
	f32 s = sin(angle);

	_matrix_identity(matrix);
	matrix[0]=c;
	matrix[1]=s;
	matrix[4]=-s;
	matrix[5]=c;
}

void
_matrix_translation(const f32 *t, f32 *matrix)
{
	_matrix_identity(matrix);
	matrix[12]=t[0];
	matrix[13]=t[1];
	matrix[14]=t[2];
}


void
_quaternion_rotate_x(const f32 *quat, f32 angle, f32 *result) {
	angle *= 3.14159265 / 180.0;
	f32 s = sin(angle);
	f32 c = cos(angle);

	result[0] = quat[0] * c + quat[3] * s;
	result[1] = quat[1] * c + quat[2] * s;
	result[2] = quat[2] * c - quat[1] * s;
	result[3] = quat[3] * c - quat[0] * s;
}


void
_matrix_lookAt(f32 *eye, f32 *center, f32 *up, f32 *out)
{
    _matrix_identity(out);

    f32 f[3];

    for(int i=0; i<3; i++) {
    	f[i]=center[i] - eye[i];
    }

    _normalize(f);
    _normalize(up);


    f32 s[3];
    _cross(f, up, s);
    _normalize(s);

    _cross(s, f, up);

    out[0] = s[0];
    out[1] = up[0];
    out[2] = -f[0];

    out[4] = s[1];
    out[5] = up[1];
    out[6] = -f[1];

    out[8] = s[2];
    out[9] = up[2];
    out[10] = -f[2];

    out[12] = -_dot(s, eye);
    out[13] = -_dot(up, eye);
    out[14] = _dot(f, eye);
}


static f32
_abs(f32 x)
{
	return x>0?x:-x;
}

static void
_normalize(f32 *x)
{
	f32 n = _dot(x, x);
	if(n != 0) {
		n=sqrt(n);
		x[0]/=n;
		x[1]/=n;
		x[2]/=n;
	}
}

static f32
_dot(f32 *x, f32 *y)
{
	return x[0]*y[0]+x[1]*y[1]+x[2]*y[2];
}

static void
_cross(f32 *x, f32 *y, f32 *cross)
{  
    cross[0] = x[1] * y[2] - x[2] * y[1]; 
    cross[1] = x[2] * y[0] - x[0] * y[2]; 
    cross[2] = x[0] * y[1] - x[1] * y[0];
}

void
_matrix_display(f32 *m)
{
	fprintf(stderr, "\n\n╭───────────────────────────────────────────────╮");
	for(int i=0; i<4; i++) {
		fprintf(stderr, "\n│");
		for(int j=0; j<4; j++) {
			fprintf(stderr, "%f\t", m[i+4*j]);
		}
		fprintf(stderr, (i<3)?"│\n│\t\t\t\t\t\t\t\t\t\t\t\t│":
			"│\n╰───────────────────────────────────────────────╯");
	}
	fprintf(stderr, "\n\n");
}

//╭╮╰╯ │