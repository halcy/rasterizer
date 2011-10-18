/**
 * Colour handling.
 * (c) L. Diener 2010
 * TODO: Since the alpha channel is currently unused, nearly all functions
 * completely ignore it.
 */

#include "colours.h"

#include <math.h>
#include <stdio.h>

inline colour makeColour(scalar r, scalar g, scalar b) {
	colour c;
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = ((scalar)1);
	return c;
}

inline colour makeColourA(scalar r, scalar g, scalar b, scalar a) {
	colour c;
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;
	return c;
}

inline colour grey(scalar v) {
	colour c;
	c.r = v;
	c.g = v;
	c.b = v;
	c.a = ((scalar)1);
	return c;	
}

inline void clip(colour * c) {
	c->r = scalarMax( scalarMin( c->r, 1.0 ), 0.0 );
	c->g = scalarMax( scalarMin( c->g, 1.0 ), 0.0 );
	c->b = scalarMax( scalarMin( c->b, 1.0 ), 0.0 );
}

inline void clipped(colour * r, colour c) {
	r->r = scalarMax( scalarMin( c.r, 1.0 ), 0.0 );
	r->g = scalarMax( scalarMin( c.g, 1.0 ), 0.0 );
	r->b = scalarMax( scalarMin( c.b, 1.0 ), 0.0 );
}

inline void makePositive(colour * c) {
	c->r = scalarMin( c->r, 1.0 );
	c->g = scalarMin( c->g, 1.0 );
	c->b = scalarMin( c->b, 1.0 );
}

inline void applyGamma(colour * r, scalar gamma) {
	r->r = scalarPow( r->r, gamma );
	r->g = scalarPow( r->g, gamma );
	r->b = scalarPow( r->b, gamma );
}

inline void brighten(colour * c, scalar f) {
	c->r = c->r + f;
	c->g = c->g + f;
	c->b = c->b + f;
}

inline void brightened(colour * r, colour c, scalar f) {
	r->r = c.r + f;
	r->g = c.g + f;
	r->b = c.b + f;
}

inline void darken(colour * c, scalar f) {
	c->r = c->r - f;
	c->g = c->g - f;
	c->b = c->b - f;
}

inline void darkened(colour * r, colour c, scalar f) {
	r->r = c.r - f;
	r->g = c.g - f;
	r->b = c.b - f;
}

inline void scale(colour * c, scalar f) {
	c->r = c->r * f;
	c->g = c->g * f;
	c->b = c->b * f;
}

inline void scaled(colour * r, colour c, scalar f) {
	r->r = c.r * f;
	r->g = c.g * f;
	r->b = c.b * f;
}
inline void scaledDown(colour * r, colour c, scalar f) {
	r->r = c.r / f;
	r->g = c.g / f;
	r->b = c.b / f;
}

inline void mult(colour * c, colour a) {
	c->r = c->r * a.r;
	c->g = c->g * a.g;
	c->b = c->b * a.b;
}

inline void multiplied(colour * r, colour a, colour b) {
	r->r = a.r * b.r;
	r->g = a.g * b.g;
	r->b = a.b * b.b;
}

inline void divide(colour * c, colour a) {
	c->r = c->r / a.r;
	c->g = c->g / a.g;
	c->b = c->b / a.b;
}

inline void divided(colour * r, colour a, colour b) {
	r->r = a.r / b.r;
	r->g = a.g / b.g;
	r->b = a.b / b.b;
}

inline void additiveMix(colour * c, colour a) {
	c->r = c->r + a.r;
	c->g = c->g + a.g;
	c->b = c->b + a.b;
}

inline void additiveMixed(colour * r, colour a, colour b) {
	r->r = a.r + b.r;
	r->g = a.g + b.g;
	r->b = a.b + b.b;
}

inline bool sameColour(colour a, colour b) {
	return( a.r == b.r && a.g == b.g && a.b == b.b );
}

// Should use LAB or something but for now weighted RGB is okay I guess
inline scalar colourDist(colour a, colour b) {
	return(
		0.3 * (a.r - b.r) * (a.r - b.r) +
		0.4 * (a.g - b.g) * (a.g - b.g) +
		0.2 * (a.b - b.b) * (a.b - b.b)
	);
}

vec3 getRGB(colour c) {
	return( makeVec3( c.r*255.0f, c.g*255.0f, c.b*255.0f ) );
}

scalar wavelengths[] = {5.70, 5.40, 4.40};
scalar getComponentWaveLength(colour c, int component) {
	return( component < 3 ? wavelengths[component] : 0 );
}

colour getComponentColour(colour c, int component) {
	if( component == 0 ) {
		return makeColour( c.r, 0, 0 );
	}
	else if( component == 1 ) {
		return makeColour( 0, c.g, 0 );
	}
	else if( component == 2 ) {
		return makeColour( 0, 0, c.b );
	}
	return COLOUR_BLACK;
}

void multComponent(colour* c, int component, scalar s) {
	if( component == 0 ) {
		c->r *= s;
	}
	else if( component == 1 ) {
		c->g *= s;
	}
	else if( component == 2 ) {
		c->b *= s;
	}
}

void decay(colour* r, colour f, scalar x) {
// 	f.r = ((scalar)1) - f.r;
// 	f.g = ((scalar)1) - f.g;
// 	f.b = ((scalar)1) - f.b;
	if( !sameColour( f, COLOUR_WHITE ) ) {
		f.r *= (scalarPow( scalarE, -x * (((scalar)1) - f.r) ));
		f.g *= (scalarPow( scalarE, -x * (((scalar)1) - f.g) ));
		f.b *= (scalarPow( scalarE, -x * (((scalar)1) - f.b) ));
		r->r *= f.r;
		r->g *= f.g;
		r->b *= f.b;
	}
}

// Debugging
void colPrintf(colour c) {
	printf( "%f, %f, %f\n", c.r, c.g, c.b );
}
