/**
 * A very simple software rasterizer.
 * (c) L. Diener 2011
 */

#ifndef __RASTERIZER_H__
#define __RASTERIZER_H__

#include <float.h>

#include "buffers.h"
#include "models.h"

void rasterize(model* m, buffer* pbuf, float* zbuf);

#endif