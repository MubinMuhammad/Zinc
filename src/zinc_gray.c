#include "zinc_gray.h"
#include "zinc_types.h"

#include <stdio.h>

void zinc_gray(zinc_image *image) {
  uint32_t   p = 0;
  float      y;
  zinc_vec4  v;

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_vec4_unc(ZINC_RGB_TO_NORMALIZED, *image, v, p);

    y = (v[0] + v[1] + v[2]) / 3;

    v[0] = y;
    v[1] = y;
    v[2] = y;
 
    zinc_vec4_unc(ZINC_NORMALIZED_TO_RGB, *image, v, p);
    p += image->color_channels;
  }
}

void zinc_gray_correc(zinc_image *image) {
  uint32_t  p = 0;
  float     y;
  zinc_vec4 v;

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_vec4_unc(ZINC_RGB_TO_NORMALIZED, *image, v, p);

    y = (v[0] * 0.30 + 
         v[1] * 0.59 + 
         v[2] * 0.11 ) / 3;

    v[0] = y;
    v[1] = y;
    v[2] = y;

    zinc_vec4_unc(ZINC_RGB_TO_NORMALIZED, *image, v, p);
    p += image->color_channels;
  }
}

void zinc_gray_lumin(zinc_image *image) {
  uint32_t  p = 0;
  float     y;
  zinc_vec4 v;

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_vec4_unc(ZINC_RGB_TO_NORMALIZED, *image, v, p);

    y = (v[0] * 0.2126 + 
         v[1] * 0.7152 + 
         v[2] * 0.0722 ) / 3;

    v[0] = y;
    v[1] = y;
    v[2] = y;

    zinc_vec4_unc(ZINC_RGB_TO_NORMALIZED, *image, v, p);
    p += image->color_channels;
  }
}

void zinc_gray_light(zinc_image *image) {
  uint32_t  p = 0;
  float     y;
  float     mx;
  float     mn;
  zinc_vec4 v;

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_vec4_unc(ZINC_RGB_TO_NORMALIZED, *image, v, p);
    
    mx = max3(v[0], v[1], v[2]);
    mn = min3(v[0], v[1], v[2]);

    y = (mx + mn) / 3;

    v[0] = y;
    v[1] = y;
    v[2] = y;

    zinc_vec4_unc(ZINC_RGB_TO_NORMALIZED, *image, v, p);
    p += image->color_channels;
  }
}
