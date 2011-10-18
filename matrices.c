/**
 * Matrices, 4x4 only.
 * Matrix inverse lifted from MESA and adapted. Holy christ.
 * (c) 2010 L. Diener
 */

/**
 * Matrices look like so:
 * 0  1  2  3
 * 4  5  6  7
 * 8  9  10 11
 * 12 13 14 15
 */

#include "matrices.h"
#include "vectors.h"

void matrixMult(matrix* r, matrix a, matrix b) {
	for( int x = 0; x < 4; x++ ) {
		for( int y = 0; y < 4; y++ ) {
			r->v[4*y+x] =
				a.v[0+4*y] * b.v[0+x] +
				a.v[1+4*y] * b.v[4+x] +
				a.v[2+4*y] * b.v[8+x] +
				a.v[3+4*y] * b.v[12+x];
		}
	}
}

bool matrixInverse(matrix* r, matrix in) {
	scalar inv[16];
	scalar det;

	scalar* m = in.v;
	
	inv[0] =
		m[5]*m[10]*m[15] -
		m[5]*m[11]*m[14] -
		m[9]*m[6]*m[15] +
		m[9]*m[7]*m[14] +
		m[13]*m[6]*m[11] -
		m[13]*m[7]*m[10];
	inv[4] =
		-m[4]*m[10]*m[15] +
		m[4]*m[11]*m[14] +
		m[8]*m[6]*m[15] -
		m[8]*m[7]*m[14] -
		m[12]*m[6]*m[11] +
		m[12]*m[7]*m[10];
	inv[8] =
		m[4]*m[9]*m[15] -
		m[4]*m[11]*m[13] -
		m[8]*m[5]*m[15] +
		m[8]*m[7]*m[13] +
		m[12]*m[5]*m[11] -
		m[12]*m[7]*m[9];
	inv[12] =
		-m[4]*m[9]*m[14] +
		m[4]*m[10]*m[13] +
		m[8]*m[5]*m[14] -
		m[8]*m[6]*m[13] -
		m[12]*m[5]*m[10] +
		m[12]*m[6]*m[9];
	inv[1] =
		-m[1]*m[10]*m[15] +
		m[1]*m[11]*m[14] +
		m[9]*m[2]*m[15] -
		m[9]*m[3]*m[14] -
		m[13]*m[2]*m[11] +
		m[13]*m[3]*m[10];
	inv[5] =
		m[0]*m[10]*m[15] -
		m[0]*m[11]*m[14] -
		m[8]*m[2]*m[15] +
		m[8]*m[3]*m[14] +
		m[12]*m[2]*m[11] -
		m[12]*m[3]*m[10];
	inv[9] =
		-m[0]*m[9]*m[15] +
		m[0]*m[11]*m[13] +
		m[8]*m[1]*m[15] -
		m[8]*m[3]*m[13] -
		m[12]*m[1]*m[11] +
		m[12]*m[3]*m[9];
	inv[13] =
		m[0]*m[9]*m[14] -
		m[0]*m[10]*m[13] -
		m[8]*m[1]*m[14] +
		m[8]*m[2]*m[13] +
		m[12]*m[1]*m[10] -
		m[12]*m[2]*m[9];
	inv[2] =
		m[1]*m[6]*m[15] -
		m[1]*m[7]*m[14] -
		m[5]*m[2]*m[15] +
		m[5]*m[3]*m[14] +
		m[13]*m[2]*m[7] -
		m[13]*m[3]*m[6];
	inv[6] =
		-m[0]*m[6]*m[15] +
		m[0]*m[7]*m[14] +
		m[4]*m[2]*m[15] -
		m[4]*m[3]*m[14] -
		m[12]*m[2]*m[7] +
		m[12]*m[3]*m[6];
	inv[10] =
		m[0]*m[5]*m[15] -
		m[0]*m[7]*m[13] -
		m[4]*m[1]*m[15] +
		m[4]*m[3]*m[13] +
		m[12]*m[1]*m[7] -
		m[12]*m[3]*m[5];
	inv[14] =
		-m[0]*m[5]*m[14] +
		m[0]*m[6]*m[13] +
		m[4]*m[1]*m[14] -
		m[4]*m[2]*m[13] -
		m[12]*m[1]*m[6] +
		m[12]*m[2]*m[5];
	inv[3] =
		-m[1]*m[6]*m[11] +
		m[1]*m[7]*m[10] +
		m[5]*m[2]*m[11] -
		m[5]*m[3]*m[10] -
		m[9]*m[2]*m[7] +
		m[9]*m[3]*m[6];
	inv[7] =
		m[0]*m[6]*m[11] -
		m[0]*m[7]*m[10] -
		m[4]*m[2]*m[11] +
		m[4]*m[3]*m[10] +
		m[8]*m[2]*m[7] -
		m[8]*m[3]*m[6];
	inv[11] =
		-m[0]*m[5]*m[11] +
		m[0]*m[7]*m[9] +
		m[4]*m[1]*m[11] -
		m[4]*m[3]*m[9] -
		m[8]*m[1]*m[7] +
		m[8]*m[3]*m[5];
	inv[15] =
		m[0]*m[5]*m[10] -
		m[0]*m[6]*m[9] -
		m[4]*m[1]*m[10] +
		m[4]*m[2]*m[9] +
		m[8]*m[1]*m[6] -
		m[8]*m[2]*m[5];

	det =
		m[0]*inv[0] +
		m[1]*inv[4] +
		m[2]*inv[8] +
		m[3]*inv[12];
		
	if (det == 0) {
		return false;
	}

	det = 1.0 / det;

	for(int i = 0; i < 16; i++) {
		r->v[i] = inv[i] * det;
	}

	return true;
}

void matrixTranspose(matrix* r, matrix m) {
	r->v[0] = m.v[0];
	r->v[1] = m.v[4];
	r->v[2] = m.v[8];
	r->v[3] = m.v[12];
	r->v[4] = m.v[1];
	r->v[5] = m.v[5];
	r->v[6] = m.v[9];
	r->v[7] = m.v[13];
	r->v[8] = m.v[2];
	r->v[9] = m.v[6];
	r->v[10] = m.v[10];
	r->v[11] = m.v[14];
	r->v[12] = m.v[3];
	r->v[13] = m.v[7];
	r->v[14] = m.v[11];
	r->v[15] = m.v[15];
}

void matrixNormalTransform(matrix* r, matrix m) {
	m.v[3] = 0;
	m.v[7] = 0;
	m.v[11] = 0;
	m.v[15] = 1;
	matrix t;
	matrixInverse( &t, m );
	matrixTranspose( r, t );
}

void matrixApply(vec3* r, matrix a, vec3 b) {
	r->x = a.v[0] * b.x + a.v[1] * b.y + a.v[2]  * b.z + a.v[3];
	r->y = a.v[4] * b.x + a.v[5] * b.y + a.v[6]  * b.z + a.v[7];
	r->z = a.v[8] * b.x + a.v[9] * b.y + a.v[10] * b.z + a.v[11];
}

void matrixApplyNormal(vec3* r, matrix a, vec3 b) {
	r->x = a.v[0] * b.x + a.v[1] * b.y + a.v[2]  * b.z;
	r->y = a.v[4] * b.x + a.v[5] * b.y + a.v[6]  * b.z;
	r->z = a.v[8] * b.x + a.v[9] * b.y + a.v[10] * b.z;
	normalize3( r );
}
#include <stdio.h>
void matrixApplyPerspective(vec3* r, matrix a, vec3 b) {
	r->x = a.v[0] * b.x + a.v[1] * b.y + a.v[2]  * b.z + a.v[3];
	r->y = a.v[4] * b.x + a.v[5] * b.y + a.v[6]  * b.z + a.v[7];
	r->z = a.v[8] * b.x + a.v[9] * b.y + a.v[10] * b.z + a.v[11];
	scalar w = a.v[12] * b.x + a.v[13] * b.y + a.v[14] * b.z + a.v[15];
	
	if(scalarAbs(w) > 0.00001) {
		r->x /= w;
		r->y /= w;
		r->z /= w;
	}
}

void matrixId(matrix* m) {
	m->v[0]  = 1; m->v[1]  = 0; m->v[2]  = 0; m->v[3]  = 0;
	m->v[4]  = 0; m->v[5]  = 1; m->v[6]  = 0; m->v[7]  = 0;
	m->v[8]  = 0; m->v[9]  = 0; m->v[10] = 1; m->v[11] = 0;
	m->v[12] = 0; m->v[13] = 0; m->v[14] = 0; m->v[15] = 1;
}

void matrixScale(matrix* m, scalar x, scalar y, scalar z) {
	m->v[0]  = ((scalar)1)/x; m->v[1]  = 0; m->v[2]  = 0; m->v[3]  = 0;
	m->v[4]  = 0; m->v[5]  = ((scalar)1)/y; m->v[6]  = 0; m->v[7]  = 0;
	m->v[8]  = 0; m->v[9]  = 0; m->v[10] = ((scalar)1)/z; m->v[11] = 0;
	m->v[12] = 0; m->v[13] = 0; m->v[14] = 0; m->v[15] = 1;
}

void matrixRotY(matrix* m, scalar a) {
	a = -a;
	m->v[0]  = scalarCos(a); m->v[1]  = 0; m->v[2]  = scalarSin(a); m->v[3]  = 0;
	m->v[4]  = 0; m->v[5]  = 1; m->v[6]  = 0; m->v[7]  = 0;
	m->v[8]  = -scalarSin(a); m->v[9]  = 0; m->v[10] = scalarCos(a); m->v[11] = 0;
	m->v[12] = 0; m->v[13] = 0; m->v[14] = 0; m->v[15] = 1;
}

void matrixTranslate(matrix* m, scalar x, scalar y, scalar z) {
	m->v[0]  = 1; m->v[1]  = 0; m->v[2]  = 0; m->v[3]  = -x;
	m->v[4]  = 0; m->v[5]  = 1; m->v[6]  = 0; m->v[7]  = -y;
	m->v[8]  = 0; m->v[9]  = 0; m->v[10] = 1; m->v[11] = -z;
	m->v[12] = 0; m->v[13] = 0; m->v[14] = 0; m->v[15] = 1;
} 

void matrixPerspective(matrix* m, scalar degrees, scalar aspect, scalar near, scalar far) {
        scalar f = ((scalar)1) / scalarTan(degrees*scalarPI/((scalar)180)/((scalar)2));
	m->v[0]  = f/aspect; m->v[1] = 0;  m->v[2]  = 0;                     m->v[3]  = 0;
	m->v[4]  = 0;        m->v[5] = f;  m->v[6]  = 0;                     m->v[7]  = 0;
	m->v[8]  = 0;        m->v[9] = 0;  m->v[10] = (far+near)/(near-far); m->v[11] = ((scalar)2)*far*near/(near-far);
	m->v[12] = 0;        m->v[13] = 0; m->v[14] = ((scalar)-1);          m->v[15] = 0;
}