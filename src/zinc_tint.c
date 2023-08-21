#include "zinc_tint.h"

void zinc_tint(zinc_image *image, float const *colors) {
  uint32_t  p = 0;
  zinc_vec4 v;

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_vec4_unc(ZINC_RGB_TO_NORMALIZED, *image, v, p);

    v[0] *= colors[0];
    v[1] *= colors[1];
    v[2] *= colors[2];

    zinc_vec4_unc(ZINC_NORMALIZED_TO_RGB, *image, v, p);
    p += image->color_channels;
  }
}

void zinc_tint_equal(zinc_image *image, float factor) {
  uint32_t  p = 0;
  zinc_vec4 v;

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_vec4_unc(ZINC_RGB_TO_NORMALIZED, *image, v, p);

    v[0] *= factor;
    v[1] *= factor;
    v[2] *= factor;

    zinc_vec4_unc(ZINC_NORMALIZED_TO_RGB, *image, v, p);
    p += image->color_channels;
  }
}

void zinc_tint_invert(zinc_image *image) {
  uint32_t  p = 0;
  zinc_vec4 v;

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_vec4_unc(ZINC_RGB_TO_NORMALIZED, *image, v, p);

    v[0] = 1.0f - v[0];
    v[1] = 1.0f - v[1];
    v[2] = 1.0f - v[2];

    zinc_vec4_unc(ZINC_NORMALIZED_TO_RGB, *image, v, p);
    p += image->color_channels;
  }
}
