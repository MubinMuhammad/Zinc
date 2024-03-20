#include "zinc_tint.h"

void zinc_tint(struct zinc_image *image, float *colors, uint32_t px_idx) {
  image->data[px_idx + 0] = CLAMP((uint32_t)image->data[px_idx + 0] * colors[0], 0, 255);
  image->data[px_idx + 1] = CLAMP((uint32_t)image->data[px_idx + 1] * colors[1], 0, 255);
  image->data[px_idx + 2] = CLAMP((uint32_t)image->data[px_idx + 2] * colors[2], 0, 255);
}

void zinc_tint_equal(struct zinc_image *image, float factor, uint32_t px_idx) {
  zinc_tint(image, (float[3]){factor, factor, factor}, px_idx);
}

void zinc_tint_invert(struct zinc_image *image, uint32_t px_idx) {
  image->data[px_idx + 0] = 255 - image->data[px_idx + 0];
  image->data[px_idx + 1] = 255 - image->data[px_idx + 1];
  image->data[px_idx + 2] = 255 - image->data[px_idx + 2];
}
