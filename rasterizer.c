/**
 * A very simple software rasterizer.
 * (c) L. Diener 2011
 */

#include "rasterizer.h"

model globalModel;

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
	int width = pbuf->width;
	int height = pbuf->size;
	
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
		t.xmax = fmin(width, t.xmax);
		t.ymin = fmax(0, t.ymin);
		t.ymax = fmin(height, t.ymax);

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
