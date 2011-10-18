/**
 * Shared memory buffers.
 * 2011 edit: Except not because NaCL.
 * (c) L. Diener 2010
 */

#include "buffers.h"
#include "vectors.h"
#include "bmp_handler.h"

#include <stdlib.h>
/*#include <sys/ipc.h>
#include <sys/shm.h>*/

buffer makeBuffer(int width, int height) {
	buffer b;
	b.width = width;
	b.firstLine = 0;
	b.size = height;
	/*b.memid = shmget(
		(key_t)0,
		sizeof( colour ) * width * height,
		0660|IPC_CREAT|IPC_PRIVATE
	);*/
	b.data = (colour*)malloc(sizeof( colour ) * width * height);
	return b;
}

buffer partialBuffer(buffer b, int index, int parts) {
	buffer p = b;
	p.size /= parts;
	p.firstLine += p.size * index;
	return p;
}

inline void setPixel(buffer b, int x, int y, colour c) {
	b.data[x+b.width*y] = c;
}

void expose(buffer b, scalar factor, scalar gamma) {
	for( int x = 0; x < b.width; x++ ) {
		for( int y = b.firstLine; y < b.size; y++ ) {
			scale( &b.data[x+b.width*y], factor );
			applyGamma( &b.data[x+b.width*y], 1.0f / gamma);
			clip( &b.data[x+b.width*y] );
		}
	}
}

void clear(buffer b) {
	for( int x = 0; x < b.width; x++ ) {
		for( int y = b.firstLine; y < b.size; y++ ) {
			b.data[x+b.width*y] = COLOUR_BLACK;
		}
	}
}

void writeToImage(buffer b, const char* filename) {
	bmp_init( filename, b.width, b.size );
	for( int y = b.firstLine; y < b.size; y++ ) {
		for( int x = 0; x < b.width; x++ ) {
			vec3 c = getRGB( b.data[x+b.width*y] );
			bmp_pixel( (int)c.x, (int)c.y, (int)c.z );
		}
	}
	bmp_close();
}

void freeBuffer(buffer b) {
	/*shmdt( b.data );	
	shmctl( b.memid, IPC_RMID, 0 );*/
	free(b.data);
}
