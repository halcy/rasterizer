/**
 * A very simple soft rasterizer.
 * (c) L. Diener 2011
 */

#include <GL/gl.h>
#include <GL/glut.h>

#define WIDTH 320
#define HEIGHT 240

#include "rasterizer.h"

buffer frameBuffer;
model globalModel;
float rotAngle;

void display() {
	clear(frameBuffer);

	// Initialize z buffer
	float* zbuf = (float*)malloc( sizeof(float) * WIDTH * HEIGHT );
	for( int y = 0; y < HEIGHT; y++ ) {
		for( int x = 0; x < WIDTH; x++ ) {
			zbuf[y*WIDTH+x] = FLT_MIN;
		}
	}

	matrix transMatrix, rotMatrixA, mvMatrixO;
	matrixTranslate(&transMatrix, 0, 0, 6);
	matrixRotY(&rotMatrixA, rotAngle);
	matrixMult(&mvMatrixO, transMatrix, rotMatrixA);

	matrix pMatrixO;
	matrixPerspective(&pMatrixO, 45, 4.0/3.0, 1.0, 32.0 );

	applyTransforms(&globalModel, mvMatrixO, pMatrixO);
	shade(&globalModel, 5, 5, 5);

	rasterize(&globalModel, &frameBuffer, zbuf);

	// Copy img's buffer to the screen
	glDrawPixels(WIDTH, HEIGHT, GL_RGBA, GL_FLOAT, frameBuffer.data);
	rotAngle += 0.02;
	
	// Leaking memory is rude.
	free( zbuf );

	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y) {
	switch(key) {
		case 27:
			exit(0);
		break;

		case 's':
			writeToImage(frameBuffer, "out.bmp");
		break;

		default:
		break;
	}
}

int main(int argc, char **argv) {

	globalModel = makeModelFromMeshFile("suzanne.raw");

	frameBuffer = makeBuffer(320,240);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Rasterizer");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);

	glutMainLoop();

  return 0;
}
