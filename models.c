/**
 * Mesh/model managment functions. Include transforms and flat shading.
 * Raw mesh loader taken from a computer graphics class.
 * (c) L. Diener 2011
 */

#include "models.h"

void readRawMesh(model* newModel, const char *filename) {
	FILE *file = fopen(filename, "rb");
	size_t res;

	int triangleCount;
	if (file) {
		if(!sizeof(float) == 4) {
			fprintf(stderr, "No 4 byte floats.");
			exit(1);
		}
		res = fread(&triangleCount, sizeof(int), 1, file);
		newModel->mesh = (float*)malloc(triangleCount * 18 * sizeof(float));
		res = fread(newModel->mesh, 18 * sizeof(float), triangleCount, file);

		fclose(file);
	}
	else {
		fprintf(stderr, "Error: Couldn't open \"%s\".\n", filename);
		exit(1);
	}

	newModel->triangleCount = triangleCount;
}

void prepareMesh(model* newModel) {
	newModel->curTriangle = 0;

	newModel->triangles = (triangle*)malloc(sizeof(triangle) * newModel->triangleCount);
	for (int i = 0; i < newModel->triangleCount; i++) {
		newModel->triangles[i].ID = i;
	}
}

model makeModelFromMeshFile(const char* file) {
	model newModel;
	readRawMesh(&newModel, file);
	prepareMesh(&newModel);
	return newModel;
}

model makeModelFromMesh(float* renderMesh, int tris) {
	model newModel;
	newModel.mesh = (float*)malloc(sizeof(float) * tris * 18);
	memcpy(newModel.mesh, renderMesh, sizeof(float) * tris * 18);
	newModel.triangleCount = tris;
	prepareMesh(&newModel);
	return newModel;
}

void freeModel(model* m) {
	free(m->mesh);
	free(m->triangles);
}

int modelTrianglesLeft(model* m) {
	return(m->curTriangle != m->triangleCount);
}

triangle* modelNextTriangle(model* m) {
	return &m->triangles[m->curTriangle++];
}

int modelTriangleCount(model* m) {
	return m->triangleCount;
}

void applyTransforms(model* m, matrix mvMatrixO, matrix pMatrixO) {
	vec3 triVec;
	vec3 transVec;
	vec3 projVec;

	for(int i = 0; i < m->triangleCount; i++) {
		for(int j = 0; j < 6; j++) {
			triVec = makeVec3(
				m->mesh[m->triangles[i].ID * 18 + j*3],
				m->mesh[m->triangles[i].ID * 18 + j*3 + 1],
				m->mesh[m->triangles[i].ID * 18 + j*3 + 2]
			);

			if (j % 2 == 0) {
				matrixApply(&transVec, mvMatrixO, triVec);
				matrixApplyPerspective(&projVec, pMatrixO, transVec);

				m->triangles[i].vertices[j/2][0] = projVec.x;
				m->triangles[i].vertices[j/2][1] = projVec.y;
				m->triangles[i].vertices[j/2][2] = projVec.z;
			}
			else {
				matrixApplyNormal(&transVec, mvMatrixO, triVec);
				m->triangles[i].normals[j/2][0] = transVec.x;
				m->triangles[i].normals[j/2][1] = transVec.y;
				m->triangles[i].normals[j/2][2] = transVec.z;
			}
		}
	}

	m->curTriangle = 0;
}

void shade(model* m, float lx, float ly, float lz) {
	for(int i = 0; i != m->triangleCount; i++) {
		for(int j = 0; j < 3; j++) {
			for(int c = 0; c < 3; c++) {
				// Diffuse lighting, white.
				float Lx = lx - m->mesh[m->triangles[i].ID * 18 + j*6];
				float Ly = ly - m->mesh[m->triangles[i].ID * 18 + j*6 + 1];
				float Lz = lz - m->mesh[m->triangles[i].ID * 18 + j*6 + 2];

				float lenL = sqrt(Lx*Lx + Ly*Ly + Lz*Lz);

				Lx /= lenL;
				Ly /= lenL;
				Lz /= lenL;

				float NdotL =
					m->triangles[i].normals[j][0] * Lx +
					m->triangles[i].normals[j][1] * Ly +
					m->triangles[i].normals[j][2] * Lz;

				if (NdotL < 0.0f) {
					NdotL = 0.0f;
				}
				m->triangles[i].colors[j][c] = NdotL;
			}
		}
	}
}
