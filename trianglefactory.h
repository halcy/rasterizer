/**
 * Mesh managment functions. Raw mesh loader taken from a computer graphics class.
 * (c) L. Diener 2011
 */

#include <stdio.h>
#include <string.h>

#include <list>
#include <algorithm>
#include "assert.h"
#include "math.h"

#include "matrices.h"

using namespace std;

struct Triangle {
	float vertices[3][3];
	float normals[3][3];

	float colors[3][3];

	int ID;
};

int readRawMesh(const char *filename, float *&mesh) {
	FILE *file = fopen(filename, "rb");
	size_t res;

	int triangleCount;
	if (file) {
		assert(sizeof(float) == 4);

		res = fread(&triangleCount, sizeof(int), 1, file);
		mesh = new float[triangleCount * 18];
		res = fread(mesh, 18 * sizeof(float), triangleCount, file);

		fclose(file);
	}
	else {
		fprintf(stderr, "Error: Couldn't open \"%s\".\n", filename);
		exit(1);
	}

	return triangleCount;
}

class TriangleFactory {

public:
	TriangleFactory(char* file) {
		triangleCount = readRawMesh("suzanne.raw", mesh);
		prepareMesh();
	}

	TriangleFactory(float* renderMesh, int triangles) {
		mesh = renderMesh;
		triangleCount = triangles;
		prepareMesh();
	}

	void prepareMesh() {
		curTriangle = 0;

		Triangle *t;
		triangles = (Triangle*)malloc(sizeof(Triangle) * triangleCount);
		for (int i = 0; i < triangleCount; i++) {
			t = &triangles[i];

			t->vertices[0][0] = mesh[i*18];
			t->vertices[0][1] = mesh[i*18 + 1];
			t->vertices[0][2] = mesh[i*18 + 2];

			t->vertices[1][0] = mesh[i*18 + 6];
			t->vertices[1][1] = mesh[i*18 + 7];
			t->vertices[1][2] = mesh[i*18 + 8];

			t->vertices[2][0] = mesh[i*18 + 12];
			t->vertices[2][1] = mesh[i*18 + 13];
			t->vertices[2][2] = mesh[i*18 + 14];

			t->vertices[0][3] = mesh[i*18];
			t->vertices[0][4] = mesh[i*18 + 1];
			t->vertices[0][5] = mesh[i*18 + 2];

			t->vertices[1][6] = mesh[i*18 + 6];
			t->vertices[1][7] = mesh[i*18 + 7];
			t->vertices[1][8] = mesh[i*18 + 8];

			t->vertices[2][9] = mesh[i*18 + 12];
			t->vertices[2][10] = mesh[i*18 + 13];
			t->vertices[2][11] = mesh[i*18 + 14];
			
			t->colors[0][0] = 1.0;
			t->colors[0][1] = 0.0;
			t->colors[0][2] = 0.0;

			t->colors[1][0] = 0.0;
			t->colors[1][1] = 1.0;
			t->colors[1][2] = 0.0;

			t->colors[2][0] = 0.0;
			t->colors[2][1] = 0.0;
			t->colors[2][2] = 1.0;

			t->ID = i;
		}

		rotation = 0.0f;
		rotx = 0.0f;
	}

	~TriangleFactory()
	{
		free(mesh);
		free(triangles);
	}

	int empty() {
		return(curTriangle == triangleCount);
	}

	Triangle* fetch() {
		return &triangles[curTriangle++];
	}

	int getTriangleCount() {
		return triangleCount;
	}

	void transform() {
		matrix transMatrix, rotMatrixA, mvMatrixO;
		matrixTranslate(&transMatrix, 0, 0, 6);
		matrixRotY(&rotMatrixA, rotation);
		matrixMult(&mvMatrixO, transMatrix, rotMatrixA);

		matrix pMatrixO;
		matrixPerspective(&pMatrixO, 45, 4.0/3.0, 1.0, 32.0 );

		vec3 triVec;
		vec3 transVec;
		vec3 projVec;
		for(int i = 0; i < triangleCount; i++) {
			for(int j = 0; j < 6; j++) {
				triVec = makeVec3(
					mesh[triangles[i].ID * 18 + j*3],
					mesh[triangles[i].ID * 18 + j*3 + 1],
					mesh[triangles[i].ID * 18 + j*3 + 2]
				);

				if (j % 2 == 0) {
					matrixApply(&transVec, mvMatrixO, triVec);
					matrixApplyPerspective(&projVec, pMatrixO, transVec);

					triangles[i].vertices[j/2][0] = projVec.x;
					triangles[i].vertices[j/2][1] = projVec.y;
					triangles[i].vertices[j/2][2] = projVec.z;
				}
				else {
					matrixApplyNormal(&transVec, mvMatrixO, triVec);
					triangles[i].normals[j/2][0] = transVec.x;
					triangles[i].normals[j/2][1] = transVec.y;
					triangles[i].normals[j/2][2] = transVec.z;
				}
			}
		}
		
		curTriangle = 0;
	}

	void shade(float lx, float ly, float lz) {
		for(int i = 0; i != triangleCount; i++) {
			for(int j = 0; j < 3; j++) {
				for(int c = 0; c < 3; c++) {
					// Apply a very basic diffuse lighting

					float Lx = lx - mesh[triangles[i].ID * 18 + j*6];
					float Ly = ly - mesh[triangles[i].ID * 18 + j*6 + 1];
					float Lz = lz - mesh[triangles[i].ID * 18 + j*6 + 2];

					float lenL = sqrt(Lx*Lx + Ly*Ly + Lz*Lz);

					Lx /= lenL;
					Ly /= lenL;
					Lz /= lenL;

					float NdotL =
						triangles[i].normals[j][0] * Lx +
						triangles[i].normals[j][1] * Ly +
						triangles[i].normals[j][2] * Lz;

					if (NdotL < 0.0f) {
						NdotL = 0.0f;
					}
					triangles[i].colors[j][c] = NdotL;
				}
			}
		}
	}
	
	void zorder() {
		// qsort(compareZ);
	}

	void rotate(float r) {
		rotation += r;
	}

	void tilt(float t)
	{
		rotx += t;
	}

private:
 
	float *mesh;
	int triangleCount;
	int curTriangle;
	
	Triangle* triangles;

	float rotation;
	float rotx;

protected:
	static int compareZ(const Triangle *t1, const Triangle *t2) {
		return (
			t1->vertices[0][2] +
			t1->vertices[1][2] +
			t1->vertices[2][2]
			>
			t2->vertices[0][2] +
			t2->vertices[1][2] +
			t2->vertices[2][2]
		);
	}

};
