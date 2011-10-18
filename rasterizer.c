/**
 * A very simple soft rasterizer.
 * (c) L. Diener 2011
 */

#include <GL/gl.h>
#include <GL/glut.h>

#include <float.h>

#include "buffers.h"

#define WIDTH 320
#define HEIGHT 240

#define ROTATE 1

#include "models.h"

model globalModel;
buffer frameBuffer;

int width;
int height;

int fullscreen = 0;

#define SCREEN_X(p) (((p)+1)*((float)(width/2)))
#define SCREEN_Y(p) (((p)+1)*((float)(height/2)))

typedef struct tri {
	float sx[3];
	float sy[3];
	float nx[3];
	float ny[3];
	float b[3];
	int xmin;
	int xmax;
	int ymin;
	int ymax;
} tri;

void rasterize(model* m, buffer* pbuf, float* zbuf) {

	// Status variables.
	tri t;
	triangle* modelTri;
	float d1;
	float d2;
	float d3;
	float r;
	float g;
	float b;
	int x;
	int y;
	float z;
	int i;
	float l;

	// The actual rasterizer.
	while(modelTrianglesLeft(m)) {
		modelTri = modelNextTriangle(m);
		
		// Backface cull
		if(
			(modelTri->vertices[1][0] - modelTri->vertices[0][0]) *
			(modelTri->vertices[2][1] - modelTri->vertices[0][1]) -
			(modelTri->vertices[2][0] - modelTri->vertices[0][0]) *
			(modelTri->vertices[1][1] - modelTri->vertices[0][1])
			< 0
		) {
			continue;
		}

		// Lines of the form: d = nx * ( x - sx ) + ny * ( y - sy )
		for( i = 0; i < 3; i++ ) {
			t.sx[i] = SCREEN_X( modelTri->vertices[i][0] );
			t.sy[i] = SCREEN_Y( modelTri->vertices[i][1] );
		}

		// Normals
		for( i = 0; i < 3; i++ ) {
			t.nx[i] = -(t.sy[(i+1)%3] - t.sy[i]);
			t.ny[i] =  (t.sx[(i+1)%3] - t.sx[i]);
			l = sqrt( t.nx[i] * t.nx[i] + t.ny[i] * t.ny[i] );
			t.nx[i] /= l;
			t.ny[i] /= l;
		}

		// For barycentric coordinates
		for( i = 0; i < 3; i++ ) {
			t.b[i] = t.nx[i] * ( t.sx[(i+2)%3] - t.sx[i] ) + t.ny[i] * ( t.sy[(i+2)%3] - t.sy[i] );
		}

		// Bounding rectangles.
		t.xmin = floor( fmin( fmin( t.sx[0], t.sx[1] ), t.sx[2] ) );
		t.ymin = floor( fmin( fmin( t.sy[0], t.sy[1] ), t.sy[2] ) );
		t.xmax = ceil( fmax( fmax( t.sx[0], t.sx[1] ), t.sx[2] ) );
		t.ymax = ceil( fmax( fmax( t.sy[0], t.sy[1] ), t.sy[2] ) );

		// Clip and possibly reject.
		t.xmin = fmax(0, t.xmin);
		t.xmax = fmin(WIDTH, t.xmax);
		t.ymin = fmax(0, t.ymin);
		t.ymax = fmin(HEIGHT, t.ymax);

		if(t.ymin > t.ymax || t.xmin > t.xmax) {
			continue;
		}
		// Draw pixels inside, if need be
		for( y = t.ymin; y < t.ymax; y++ ) {
			for( x = t.xmin; x < t.xmax; x++ ) {
				d1 = t.nx[0] * ( x - t.sx[0] ) + t.ny[0] * ( y - t.sy[0] );
				if( d1 >= 0 ) {
					d2 = t.nx[1] * ( x - t.sx[1] ) + t.ny[1] * ( y - t.sy[1] );
					if( d2 >= 0 ) {
						d3 = t.nx[2] * ( x - t.sx[2] ) + t.ny[2] * ( y - t.sy[2] );
						if( d3 >= 0 ) {
							d1 /= t.b[0];
							d2 /= t.b[1];
							d3 /= t.b[2];

							// Z test
							z =
								1.0f / modelTri->vertices[0][2] * d2 +
								1.0f / modelTri->vertices[1][2] * d3 +
								1.0f / modelTri->vertices[2][2] * d1;

							if( z > zbuf[y*width+x] ) {
								zbuf[y*width+x] = z;

								r =
									modelTri->colors[0][0] * d2 +
									modelTri->colors[1][0] * d3 +
									modelTri->colors[2][0] * d1;
								g =
									modelTri->colors[0][1] * d2  +
									modelTri->colors[1][1] * d3  +
									modelTri->colors[2][1] * d1;
								b =
									modelTri->colors[0][2] * d2 +
									modelTri->colors[1][2] * d3 +
									modelTri->colors[2][2] * d1;

								
								setPixel(*pbuf, x, y, makeColour(r,g,b));
							}
						}
					}
				}
			}
		}
	}
}

void display() {
	clear(frameBuffer);

	// Initialize z buffer
	float* zbuf = (float*)malloc( sizeof(float) * width * height );
	for( int y = 0; y < height; y++ ) {
		for( int x = 0; x < width; x++ ) {
			zbuf[y*width+x] = FLT_MIN;
		}
	}

	matrix transMatrix, rotMatrixA, mvMatrixO;
	matrixTranslate(&transMatrix, 0, 0, 6);
	matrixRotY(&rotMatrixA, 2.0);
	matrixMult(&mvMatrixO, transMatrix, rotMatrixA);

	matrix pMatrixO;
	matrixPerspective(&pMatrixO, 45, 4.0/3.0, 1.0, 32.0 );
		
	applyTransforms(&globalModel, mvMatrixO, pMatrixO);
	shade(&globalModel, 5, 5, 5);

	rasterize(&globalModel, &frameBuffer, zbuf);

	// Copy img's buffer to the screen
	glDrawPixels(width, height, GL_RGBA, GL_FLOAT, frameBuffer.data);

	// Leaking memory is rude.
	free( zbuf );

	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y)
{
  switch(key)
    {
    case 27:
      exit(0);
      break;
    case 'f':
      if (fullscreen)
	glutReshapeWindow(WIDTH, HEIGHT);
      else
	glutFullScreen();
      fullscreen = !fullscreen;
      break;
    case 's':
	writeToImage(frameBuffer, "out.bmp");
      break;
    default:
      break;
    }
}

void arrow_keys(int a_keys, int x, int y)
{
  switch(a_keys)
    {
    case GLUT_KEY_UP:
      if (!ROTATE)
	display();
      break;
    case GLUT_KEY_DOWN:
      if (!ROTATE)
	display();
      break;
    case GLUT_KEY_LEFT:
      if (!ROTATE)
	{
	  display();
	}
      break;
    case GLUT_KEY_RIGHT:
      if (!ROTATE)
	{
	  display();
	}
      break;
    default:
      break;
    }
}

int main(int argc, char **argv) {

	globalModel = makeModelFromMeshFile("suzanne.raw");
	
	frameBuffer = makeBuffer(320,240);

	width = WIDTH;
	height = HEIGHT;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Rasterizer");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(arrow_keys);

	if (ROTATE) {
		glutIdleFunc(display);
	}

	glutMainLoop();

  return 0;
}
