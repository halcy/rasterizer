/**
 * Vector maths.
 * (c) L. Diener 2010
 */

#include "vectors.h"
#include <math.h>
#include <stdio.h>

// No inlining for the moment because sepples
#define inline /**/

// Generate vectors
inline vec2 makeVec2(scalar x, scalar y) {
	vec2 v;
	v.x = x;
	v.y = y;
	return v;
}

inline vec3 makeVec3(scalar x, scalar y, scalar z) {
	vec3 v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

// Vector-scalar multiplication
inline void mult2(vec2 * r, vec2 a, scalar s) {
	r->x = a.x * s;
	r->y = a.y * s;
}

inline void mult3(vec3 *  r, vec3 a, scalar s) {
	r->x = a.x * s;
	r->y = a.y * s;
	r->z = a.z * s;
}

inline void scale2(vec2 * r, scalar s) {
	r->x *= s;
	r->y *= s;
}

inline void scale3(vec3 *  r, scalar s) {
	r->x *= s;
	r->y *= s;
	r->z *= s;
}

// Vector-scalar division
inline void div2(vec2 * r, vec2 a, scalar s) {
	r->x = a.x / s;
	r->y = a.y / s;
}

inline void div3(vec3 * r, vec3 a, scalar s) {
	r->x = a.x / s;
	r->y = a.y / s;
	r->z = a.z / s;
}

inline void invScale2(vec2 * r, scalar s) {
	r->x /= s;
	r->y /= s;
}

inline void invScale3(vec3 *  r, scalar s) {
	r->x /= s;
	r->y /= s;
	r->z /= s;
}

// Inverting a vector
inline void inverted2(vec2 * r, vec2 a) {
	r->x = -a.x;
	r->y = -a.y;
}

inline void inverted3(vec3 * r, vec3 a) {
	r->x = -a.x;
	r->y = -a.y;
	r->z = -a.z;
}

inline void invert2(vec2 * r) {
	r->x = -r->x;
	r->y = -r->y;
}

inline void invert3(vec3 * r) {
	r->x = -r->x;
	r->y = -r->y;
	r->z = -r->z;
}

// Vector-vector addition
inline void add2(vec2 * r, vec2 a, vec2 b) {
	r->x = a.x + b.x;
	r->y = a.y + b.y;
}

inline void add3(vec3 * r, vec3 a, vec3 b) {
	r->x = a.x + b.x;
	r->y = a.y + b.y;
	r->z = a.z + b.z;
}

inline void addTo2(vec2 * r, vec2 a) {
	r->x += a.x;
	r->y += a.y;
}

inline void addTo3(vec3 * r, vec3 a) {
	r->x += a.x;
	r->y += a.y;
	r->z += a.z;
}

// Vector-vector subtraction
inline void sub2(vec2 * r, vec2 a, vec2 b) {
	r->x = a.x - b.x;
	r->y = a.y - b.y;
}

inline void sub3(vec3 * r, vec3 a, vec3 b) {
	r->x = a.x - b.x;
	r->y = a.y - b.y;
	r->z = a.z - b.z;
}

inline void subFrom2(vec2 * r, vec2 a) {
	r->x -= a.x;
	r->y -= a.y;
}

inline void subFrom3(vec3 * r, vec3 a) {
	r->x -= a.x;
	r->y -= a.y;
	r->z -= a.z;
}

// Vector-vector multiplication
inline scalar dot2(vec2 a, vec2 b) {
	return( a.x*b.x + a.y*b.y );
}

inline scalar dot3(vec3 a, vec3 b) {
	return( a.x*b.x + a.y*b.y + a.z*b.z );
}

inline void cross(vec3 * r, vec3 a, vec3 b) {
	r->x = a.y * b.z - a.z * b.y;
	r->y = a.z * b.x - a.x * a.z;
	r->z = a.x * b.y - a.y * a.x;
}

// Vector sizes
inline scalar length2(vec2 v) {
	return( sqrt( v.x * v.x + v.y * v.y ) );
}

inline scalar length3(vec3 v) {
	return( sqrt( v.x * v.x + v.y * v.y + + v.z * v.z ) );
}

inline scalar dist2(vec2 a, vec2 b) {
	scalar xd = a.x - b.x;
	scalar yd = a.y - b.y;
	return( sqrt( xd * xd + yd * yd ) );
}

inline scalar dist3(vec3 a, vec3 b) {
	scalar xd = a.x - b.x;
	scalar yd = a.y - b.y;
	scalar zd = a.y - b.y;
	return( sqrt( xd * xd + yd * yd + zd * zd ) );
}

// Vector normalization
inline void normalized2(vec2 * r, vec2 v) {
	scalar l = length2( v );
	div2( r, v, l );
}

inline void normalized3(vec3 * r, vec3 v) {
	scalar l = length3( v );
	div3( r, v, l );
}

inline void normalize2(vec2 * r) {
	scalar l = length2( *r );
	invScale2( r, l );
}

inline void normalize3(vec3 * r) {
	scalar l = length3( *r );
	invScale3( r, l );
}

// Projection functions
inline void projectedOnto2(vec2 * r, vec2 a, vec2 b) {
	mult2( r, a, dot2(a, b) );
	invScale2( r, length2(a) * length2(b) );
}

inline void projectedOnto3(vec3 * r, vec3 a, vec3 b) {
	mult3( r, a, dot3(a, b) );
	invScale3( r, length3(a) * length3(b) );
}

inline void projectOnto2(vec2 a, vec2 * r) {
	scalar l = length2(a) * length2(*r);
	mult2( r, a, dot2(a, *r) );
	invScale2( r, l );
}

inline void projectOnto3(vec3 a, vec3 * r) {
	scalar l = length3(a) * length3(*r);
	mult3( r, a, dot3(a, *r) );
	invScale3( r, l );
}

// Reflection functions
inline void reflected2(vec2 * r, vec2 i, vec2 n) {
	mult2( r, n, dot2( n, i ) * ((scalar)2) );
	subFrom2( r, i );
}

inline void reflected3(vec3 * r, vec3 i, vec3 n) {
	mult3( r, n, dot3( n, i ) * ((scalar)2) );
	subFrom3( r, i );
}

inline void reflect2(vec2 * r, vec2 n) {
	vec2 i = *r;
	mult2( r, n, dot2( n, i ) * ((scalar)2) );
	subFrom2( r, i );
}

inline void reflect3(vec3 * r, vec3 n) {
	vec3 i = *r;	
	mult3( r, n, dot3( n, i ) * ((scalar)2) );
	subFrom3( r, i );
}

// Polar coordinates
inline scalar vecAtan(vec2 c) {
	return scalarAtan2( c.x, c.y );
}

// Rounding error avoidance
inline void extendTo(vec3 * r, vec3 d) {
// 	if( d.x > 0 ) {
// 		r->x = nextScalar( r->x );
// 	}
// 	else {
// 		r->x = prevScalar( r->x );
// 	}
// 	if( d.y > 0 ) {
// 		r->y = nextScalar( r->y );
// 	}
// 	else {
// 		r->y = prevScalar( r->y );
// 	}
// 	if( d.z > 0 ) {
// 		r->z = nextScalar( r->z );
// 	}
// 	else {
// 		r->z = prevScalar( r->z );
// 	}
	vec3 ext;
	mult3( &ext, d, 0.1 );
	addTo3( r, ext );
}


// Debugging
void vecPrintf3(vec3 v) {
	printf( "%f, %f, %f\n", v.x, v.y, v.z );
}
