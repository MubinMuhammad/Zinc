#ifndef _ZINC_GRAY_H_
#define _ZINC_GRAY_H_

#include "zinc_image.h"

void zinc_gray(struct zinc_image *image, uint32_t px_idx);
void zinc_gray_lumin(struct zinc_image *image, uint32_t px_idx);
void zinc_gray_lightness(struct zinc_image *image, uint32_t px_idx);

#endif
