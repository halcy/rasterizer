/**
 * Scalar values.
 * (c) 2010 L. Diener
 */

#ifndef __SCALARS_H__
#define __SCALARS_H__

#include <math.h>
#include <stdlib.h>

#define scalar float
#define scalarSqrt(x) (sqrtf((x)))
#define scalarPow(x,y) (powf((x),(y)))
#define scalarMin(x,y) (fmin((x),(y)))
#define scalarMax(x,y) (fmax((x),(y)))
#define scalarAtan2(x,y) (atan2((x),(y)))
#define scalarTan(x) (tan((x)))
#define scalarCos(x) (cos((x)))
#define scalarSin(x) (sin((x)))
#define scalarACos(x) (acos(x))
#define scalarASin(x) (asin(x))
#define scalarPI M_PI
#define scalarE M_E
#define scalarInf INFINITY
#define scalarAbs(x) (((x)<0)?(-x):(x))
#define scalarLog(x) (log((x)))
#define scalarLog2(x) (log2((x)))
#define scalarMod(x,y) (fmod((x),(y)))

scalar nextScalar(scalar s);
scalar prevScalar(scalar s);
scalar scalarRand();

#endif
