/**
 * Colour handling.
 * (c) L. Diener 2010
 */

#ifndef __COLOURS_H__
#define __COLOURS_H__

#include "scalars.h"
#include "vectors.h"

#include <stdbool.h>

#define COLOUR_WHITE (makeColour( 1.0, 1.0, 1.0 ))
#define COLOUR_BLACK (makeColour( 0.0, 0.0, 0.0 ))

typedef struct colour {
	scalar r;
	scalar b;
	scalar g;
	scalar a;
} colour;

colour makeColour(scalar r, scalar g, scalar b);
colour makeColourA(scalar r, scalar g, scalar b, scalar a);
colour grey(scalar v);

void clip(colour * c);
void clipped(colour * r, colour c);

void makePositive(colour * c);

void brighten(colour * c, scalar f);
void brightened(colour * r, colour c, scalar f);

void darken(colour * c, scalar f);
void darkened(colour * r, colour c, scalar f);

void scale(colour * c, scalar f);
void scaled(colour * r, colour c, scalar f);
void scaledDown(colour * r, colour c, scalar f);

void mult(colour * c, colour a);
void multiplied(colour * r, colour a, colour b);

void divide(colour * c, colour a);
void divided(colour * r, colour a, colour b);

void additiveMix(colour * c, colour a);
void additiveMixed(colour * r, colour a, colour b);

void applyGamma(colour * r, scalar gamma);

vec3 getRGB(colour c);

scalar getComponentWaveLength(colour c, int component);
colour getComponentColour(colour c, int component);
void multComponent(colour* c, int component, scalar s);

bool sameColour(colour a, colour b);
scalar colourDist(colour a, colour b);
void decay(colour* r, colour f, scalar x);

void colPrintf(colour c);

#endif
