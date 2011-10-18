/**
 * Vector maths.
 * (c) L. Diener 2010
 */

#ifndef __VECTORS_H__
#define __VECTORS_H__

#include "scalars.h"

typedef struct vec2 {
	scalar x;
	scalar y;
} vec2;

typedef struct vec3 {
	scalar x;
	scalar y;
	scalar z;
} vec3;

vec2 makeVec2(scalar x, scalar y);
vec3 makeVec3(scalar x, scalar y, scalar z);

void mult2(vec2 * r, vec2 a, scalar s);
void mult3(vec3 * r, vec3 a, scalar s);
void scale2(vec2 * r, scalar s);
void scale3(vec3 * r, scalar s);

void div2(vec2 * r, vec2 a, scalar s);
void div3(vec3 * r, vec3 a, scalar s);
void invScale2(vec2 * r, scalar s);
void invScale3(vec3 * r, scalar s);

void inverted2(vec2 * r, vec2 a);
void inverted3(vec3 * r, vec3 a);
void invert2(vec2 * r);
void invert3(vec3 * r);

void add2(vec2 * r, vec2 a, vec2 b);
void add3(vec3 * r, vec3 a, vec3 b);
void addTo2(vec2 * r, vec2 a);
void addTo3(vec3 * r, vec3 a);

void sub2(vec2 * r, vec2 a, vec2 b);
void sub3(vec3 * r, vec3 a, vec3 b);
void subFrom2(vec2 * r, vec2 a);
void subFrom3(vec3 * r, vec3 a);

scalar dot2(vec2 a, vec2 b);
scalar dot3(vec3 a, vec3 b);

void cross(vec3 * r, vec3 a, vec3 b);

scalar length2(vec2 v);
scalar length3(vec3 v);
scalar dist2(vec2 a, vec2 b);
scalar dist3(vec3 a, vec3 b);

void normalized2(vec2 * r, vec2 v);
void normalized3(vec3 * r, vec3 v);
void normalize2(vec2 * r);
void normalize3(vec3 * r);

void reflected2(vec2 * r, vec2 i, vec2 n);
void reflected3(vec3 * r, vec3 i, vec3 n);
void reflect2(vec2 * r, vec2 n);
void reflect3(vec3 * r, vec3 n);

scalar vecAtan(vec2 c);

void extendTo(vec3 * r, vec3 d);

void vecPrintf3(vec3 v);

#endif
