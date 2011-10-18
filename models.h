#ifndef __MODELS_H__
#define __MODELS_H__

#include <stdio.h>
#include <string.h>

#include "matrices.h"

typedef struct triangle {
	float vertices[3][3];
	float normals[3][3];

	float colors[3][3];

	int ID;
} triangle;

typedef struct model {
	float* mesh;

	int triangleCount;
	int curTriangle;

	triangle* triangles;
} model;

model makeModelFromMeshFile(const char* file);
model makeModelFromMesh(float* renderMesh, int tris);
void freeModel(model* m);
int modelTrianglesLeft(model* m);
triangle* modelNextTriangle(model* m);
int modelTriangleCount(model* m);
void applyTransforms(model* m, matrix mvMatrixO, matrix pMatrixO);
void shade(model* m, float lx, float ly, float lz);

#endif
