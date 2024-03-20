#include "zinc_transform.h"

void zinc_transform_cropwcr(struct zinc_image *image, float *top_left, 
                            float *bottom_right, float *color, 
                            uint32_t x, uint32_t y, uint32_t px_idx) {
  if (!(x >= top_left[0] && x <= bottom_right[0] && y >= top_left[1] && y <= bottom_right[1])) {
    image->data[px_idx + 0] = (color[0] * 255);
    image->data[px_idx + 1] = (color[1] * 255);
    image->data[px_idx + 2] = (color[2] * 255);
  } 
}
