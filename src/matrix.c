#include "matrix.h"

matrix_t matrix_perspective(f32 fov, f32 aspect, f32 near, f32 far) {
	matrix_t matrix = matrix_identity;
    f32 tan_half_angle = tanf(fov / 2);
    matrix.m[0] = 1.0f / (aspect * tan_half_angle);
    matrix.m[5] = 1 / (tan_half_angle);
    matrix.m[10] = -(far + near) / (far - near);
    matrix.m[11] = -1;
    matrix.m[14] = (2 * far * near) / (near - far);
    matrix.m[15] = 0.0f;
    return matrix;
}

matrix_t matrix_lookAt(f32 eye[3], f32 look[3], f32 up[3]) {
    f32 s[3];
    _cross(look, up, s);
    _normalize(s);
    _cross(s, look, up);

    return (matrix_t) {{s[0], up[0], -look[0], 0,
                        s[1], up[1], -look[1], 0,
                        s[2], up[2], -look[2], 0,
                        -_dot(s, eye), -_dot(up, eye), _dot(look, eye), 1}};

}

matrix_t matrix_multiply(const matrix_t l, const matrix_t r) {
    return (matrix_t) {{l.m[0]*r.m[ 0] + l.m[4]*r.m[ 1] + l.m[ 8]*r.m[ 2] + l.m[12]*r.m[ 3],
                        l.m[1]*r.m[ 0] + l.m[5]*r.m[ 1] + l.m[ 9]*r.m[ 2] + l.m[13]*r.m[ 3],
                        l.m[2]*r.m[ 0] + l.m[6]*r.m[ 1] + l.m[10]*r.m[ 2] + l.m[14]*r.m[ 3],
                        l.m[3]*r.m[ 0] + l.m[7]*r.m[ 1] + l.m[11]*r.m[ 2] + l.m[15]*r.m[ 3],
                        l.m[0]*r.m[ 4] + l.m[4]*r.m[ 5] + l.m[ 8]*r.m[ 6] + l.m[12]*r.m[ 7],
                        l.m[1]*r.m[ 4] + l.m[5]*r.m[ 5] + l.m[ 9]*r.m[ 6] + l.m[13]*r.m[ 7],
                        l.m[2]*r.m[ 4] + l.m[6]*r.m[ 5] + l.m[10]*r.m[ 6] + l.m[14]*r.m[ 7],
                        l.m[3]*r.m[ 4] + l.m[7]*r.m[ 5] + l.m[11]*r.m[ 6] + l.m[15]*r.m[ 7],
                        l.m[0]*r.m[ 8] + l.m[4]*r.m[ 9] + l.m[ 8]*r.m[10] + l.m[12]*r.m[11],
                        l.m[1]*r.m[ 8] + l.m[5]*r.m[ 9] + l.m[ 9]*r.m[10] + l.m[13]*r.m[11],
                        l.m[2]*r.m[ 8] + l.m[6]*r.m[ 9] + l.m[10]*r.m[10] + l.m[14]*r.m[11],
                        l.m[3]*r.m[ 8] + l.m[7]*r.m[ 9] + l.m[11]*r.m[10] + l.m[15]*r.m[11],
                        l.m[0]*r.m[12] + l.m[4]*r.m[13] + l.m[ 8]*r.m[14] + l.m[12]*r.m[15],
                        l.m[1]*r.m[12] + l.m[5]*r.m[13] + l.m[ 9]*r.m[14] + l.m[13]*r.m[15],
                        l.m[2]*r.m[12] + l.m[6]*r.m[13] + l.m[10]*r.m[14] + l.m[14]*r.m[15],
                        l.m[3]*r.m[12] + l.m[7]*r.m[13] + l.m[11]*r.m[14] + l.m[15]*r.m[15]}};
}

matrix_t matrix_rotation_x(const f32 angle) {
	f32 c = cos(angle);
	f32 s = sin(angle);

	matrix_t matrix = matrix_identity;
	matrix.m[5]=c;
	matrix.m[6]=-s;
	matrix.m[9]=s;
	matrix.m[10]=c;
}

matrix_t matrix_rotation_y(const f32 angle) {
	f32 c = cos(angle);
	f32 s = sin(angle);

    matrix_t matrix = matrix_identity;
	matrix.m[0]=c;
	matrix.m[2]=s;
	matrix.m[8]=-s;
	matrix.m[10]=c;
}


matrix_t matrix_rotation_z(const f32 angle) {
	f32 c = cos(angle);
	f32 s = sin(angle);

    matrix_t matrix = matrix_identity;
	matrix.m[0]=c;
	matrix.m[1]=s;
	matrix.m[4]=-s;
	matrix.m[5]=c;
}

matrix_t matrix_translation(const f32 t[3]) {
	matrix_t matrix = matrix_identity;
	matrix.m[12]=t[0];
	matrix.m[13]=t[1];
	matrix.m[14]=t[2];
}

static void _normalize(f32 *x) {
	f32 n = _dot(x, x);
	if(n != 0) {
		n=sqrt(n);
		x[0]/=n;
		x[1]/=n;
		x[2]/=n;
	}
}

static inline f32 _dot(f32 x[3], f32 y[3]) {
	return x[0]*y[0]+x[1]*y[1]+x[2]*y[2];
}

static void _cross(f32 x[3], f32 y[3], f32 *cross)
{  
    cross[0] = x[1] * y[2] - x[2] * y[1]; 
    cross[1] = x[2] * y[0] - x[0] * y[2]; 
    cross[2] = x[0] * y[1] - x[1] * y[0];
}

void _matrix_display(matrix_t matrix) {
	fprintf(stderr, "\n\n╭───────────────────────────────────────────────╮");
	for(int i=0; i<4; i++) {
		fprintf(stderr, "\n│");
		for(int j=0; j<4; j++) {
			fprintf(stderr, "%f\t", matrix.m[i+4*j]);
		}
		fprintf(stderr, (i<3)?"│\n│\t\t\t\t\t\t\t\t\t\t\t\t│":
			"│\n╰───────────────────────────────────────────────╯");
	}
	fprintf(stderr, "\n\n");
}

//╭╮╰╯ │