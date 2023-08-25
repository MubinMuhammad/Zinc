#include "zinc_transform.h"

void zinc_transform_cropwctr(zinc_image *image, float *top_left, float *bottom_right, float *color) {
  uint32_t  p = 0;
  zinc_vec4 v;

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_vec4_unc(ZINC_RGB_TO_NORMALIZED, *image, v, p);

    if (!(PIXEL_X > top_left[0] && PIXEL_X < bottom_right[0] &&
          PIXEL_Y > top_left[1] && PIXEL_Y < bottom_right[1])) {
      v[0] = color[0];
      v[1] = color[1];
      v[2] = color[2];
    } 

    zinc_vec4_unc(ZINC_NORMALIZED_TO_RGB, *image, v, p);
    p += image->color_channels;
  }
}

void zinc_transform_cropctr(zinc_image *image, float *top_left, float *bottom_right) {
  uint32_t  p = 0;
  zinc_vec4 v;
  
  bottom_right[0] += 1;
  bottom_right[1] += 1;

  zinc_image r = {
    .color_channels = image->color_channels,
    .type = image->type,

    .width = abs((int)(bottom_right[0] - top_left[0]) - 1),
    .height = abs((int)(bottom_right[1] - top_left[1]) - 1)
  };

  printf("%d, %d", r.width, r.height);

  r.data = malloc(r.width * r.height * r.color_channels);
  uint32_t rp = 0;

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_vec4_unc(ZINC_RGB_TO_NORMALIZED, *image, v, p);

    if (PIXEL_X > top_left[0] && PIXEL_X < bottom_right[0] &&
        PIXEL_Y > top_left[1] && PIXEL_Y < bottom_right[1]) {
      *(r.data + 0 + rp) = v[0] * 255;
      *(r.data + 1 + rp) = v[1] * 255;
      *(r.data + 2 + rp) = v[2] * 255;
      
      rp += r.color_channels;
    } 

    zinc_vec4_unc(ZINC_NORMALIZED_TO_RGB, *image, v, p);
    p += image->color_channels;
  }

  *image = r;
}
