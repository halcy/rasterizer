/**
 * Scalar values.
 * (c) 2010 L. Diener
 */

#include "scalars.h"
#include <math.h>

// Scalar value functions
// Assume we're using IEEE floats or doubles.
inline scalar nextScalar(scalar s) {
	return( s + 0.0001 );
// 	return nextafter( s, s + ((scalar)1) );
}

inline scalar prevScalar(scalar s) {
	return( s - 0.0001 );
// 	return nextafter( s, s - ((scalar)1) );
}

inline scalar scalarRand() {
	return (((float)rand()) / ((float)RAND_MAX));
}