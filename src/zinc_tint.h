#ifndef _ZINC_TINT_H_
#define _ZINC_TINT_H_

#include "zinc_image.h"

void zinc_tint(struct zinc_image *image, float *colors, uint32_t px_idx);
void zinc_tint_equal(struct zinc_image *image, float factor, uint32_t px_idx);
void zinc_tint_invert(struct zinc_image *image, uint32_t px_idx);

#endif
