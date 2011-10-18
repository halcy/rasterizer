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

#include "trianglefactory.h"
TriangleFactory *factory;

Triangle *triangle;

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

void rasterize(TriangleFactory* model, buffer* pbuf, float* zbuf) {

	// Status variables.
	tri t;
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
	float ax;
	float ay;
	float bx;
	float by;

	// The actual rasterizer.
	while(not model->empty()) {
		triangle = model->fetch();

		// Backface cull
		if(
			(triangle->vertices[1][0] - triangle->vertices[0][0]) *
			(triangle->vertices[2][1] - triangle->vertices[0][1]) -
			(triangle->vertices[2][0] - triangle->vertices[0][0]) *
			(triangle->vertices[1][1] - triangle->vertices[0][1])
			< 0
		) {
			continue;
		}

		// Lines of the form: d = nx * ( x - sx ) + ny * ( y - sy )
		for( i = 0; i < 3; i++ ) {
			t.sx[i] = SCREEN_X( triangle->vertices[i][0] );
			t.sy[i] = SCREEN_Y( triangle->vertices[i][1] );
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
								1.0f / triangle->vertices[0][2] * d2 +
								1.0f / triangle->vertices[1][2] * d3 +
								1.0f / triangle->vertices[2][2] * d1;

							if( z > zbuf[y*width+x] ) {
								zbuf[y*width+x] = z;

								r =
									triangle->colors[0][0] * d2 +
									triangle->colors[1][0] * d3 +
									triangle->colors[2][0] * d1;
								g =
									triangle->colors[0][1] * d2  +
									triangle->colors[1][1] * d3  +
									triangle->colors[2][1] * d1;
								b =
									triangle->colors[0][2] * d2 +
									triangle->colors[1][2] * d3 +
									triangle->colors[2][2] * d1;

								
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

	factory->transform();
	factory->shade(5, 5, 5);

	rasterize( factory, &frameBuffer, zbuf );

	// Copy img's buffer to the screen
	glDrawPixels(width, height, GL_RGBA, GL_FLOAT, frameBuffer.data);

	if (ROTATE) {
		factory->rotate(0.01f);
	}

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
      fullscreen = not fullscreen;
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
      factory->tilt(-1.0f);
      if (not ROTATE)
	display();
      break;
    case GLUT_KEY_DOWN:
      factory->tilt(1.0f);
      if (not ROTATE)
	display();
      break;
    case GLUT_KEY_LEFT:
      if (not ROTATE)
	{
	  factory->rotate(-1.0f);
	  display();
	}
      break;
    case GLUT_KEY_RIGHT:
      if (not ROTATE)
	{
	  factory->rotate(1.0f);
	  display();
	}
      break;
    default:
      break;
    }
}

int main(int argc, char **argv)
{

	factory = new TriangleFactory("suzanne.raw");

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
