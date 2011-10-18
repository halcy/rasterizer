/**
 * Matrices, 4x4 only.
 * (c) 2010 L. Diener
 */

/**
 * Matrices look like so:
 * 0  1  2  3
 * 4  5  6  7
 * 8  9  10 11
 * 12 13 14 15
 */

#ifndef __MATRICES_H__
#define __MATRICES_H__

#include "scalars.h"
#include "vectors.h"

#include <stdbool.h>

typedef struct matrix {
	scalar v[16];
} matrix;

void matrixMult(matrix* r, matrix a, matrix b);
void matrixTranspose(matrix* r, matrix m);
bool matrixInverse(matrix* r, matrix in);
void matrixNormalTransform(matrix* r, matrix m);
void matrixApply(vec3* r, matrix a, vec3 b);
void matrixApplyNormal(vec3* r, matrix a, vec3 b);
void matrixId(matrix* m);
void matrixScale(matrix* m, scalar x, scalar y, scalar z);
void matrixRotY(matrix* m, scalar a);
void matrixTranslate(matrix* m, scalar x, scalar y, scalar z);
void matrixPerspective(matrix* m, scalar degrees, scalar aspect, scalar near, scalar far);
void matrixApplyPerspective(vec3* r, matrix a, vec3 b);

#endif
