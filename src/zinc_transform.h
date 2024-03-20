#ifndef _ZINC_TRANSFORM_H_
#define _ZINC_TRANSFORM_H_

#include "zinc_image.h"

void zinc_transform_cropwcr(struct zinc_image *image, float *top_left, 
                            float *bottom_right, float *color, 
                            uint32_t x, uint32_t y, uint32_t px_idx);

#endif
