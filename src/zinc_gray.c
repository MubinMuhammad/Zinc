#include "zinc_image.h"

void zinc_gray(struct zinc_image *image, uint32_t px_idx) {
  uint8_t y;

  for (int i = 0; i < image->width * image->height * image->channels; i += image->channels) {
    y = (image->data[i + 0] +
         image->data[i + 1] +
         image->data[i + 2]) / 3;

    image->data[i] = y;
    image->data[i + 1] = y;
    image->data[i + 2] = y;
  }
}

void zinc_gray_lumin(struct zinc_image *image, uint32_t px_idx) {
  uint8_t y;

  for (int i = 0; i < image->width * image->height * image->channels; i += image->channels) {
    y = (image->data[i + 0] * 0.2126 +
         image->data[i + 1] * 0.7152 +
         image->data[i + 2] * 0.0722) / 3;

    image->data[i] = y;
    image->data[i + 1] = y;
    image->data[i + 2] = y;
  }
}

void zinc_gray_lightness(struct zinc_image *image, uint32_t px_idx) {
  uint8_t y;
  uint8_t mn, mx;

  mn = MIN(
    MIN(image->data[px_idx], image->data[px_idx + 1]), 
    image->data[px_idx + 2]);
  mx = MAX(
    MAX(image->data[px_idx], image->data[px_idx + 1]), 
    image->data[px_idx + 2]);

  y = (mn + mx) / 2;

  image->data[px_idx + 0] = y;
  image->data[px_idx + 1] = y;
  image->data[px_idx + 2] = y;
}
