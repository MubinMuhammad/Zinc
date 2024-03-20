#include "zinc_misc.h"

void zinc_threshold(struct zinc_image *image, uint8_t intensity, uint32_t px_idx) {
  if (image->data[px_idx + 0] > intensity) {
    image->data[px_idx + 0] = 255;
    image->data[px_idx + 1] = 255;
    image->data[px_idx + 2] = 255;
    return;
  }

  image->data[px_idx + 0] = 0;
  image->data[px_idx + 1] = 0;
  image->data[px_idx + 2] = 0;
}
