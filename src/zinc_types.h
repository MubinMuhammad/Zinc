#ifndef ZINC_TYPES_H
#define ZINC_TYPES_H

#include <stdint.h>

#define ZINC_PER_PIXEL_LOOP(w, h) \
  for (int PIXEL_Y = 0; PIXEL_Y < h; PIXEL_Y++) \
    for (int PIXEL_X = 0; PIXEL_X < w; PIXEL_X++)

#define max2(a, b) a > b ? a : b;
#define min2(a, b) a < b ? a : b;

#define max3(a, b, c)  \
  a > b && a > c ? a : \
  b > a && b > c ? b : c;
#define min3(a, b, c)  \
  a < b && a < c ? a : \
  b < a && b < c ? b : c;

typedef enum zinc_image_unit {
  ZINC_RGB_TO_NORMALIZED,
  ZINC_NORMALIZED_TO_RGB

} zinc_image_unit;

typedef enum zinc_image_type {
  ZINC_JPG,
  ZINC_PNG,
  ZINC_BMP,
  ZINC_TGA
} zinc_image_type;

typedef struct zinc_image {
  int width;
  int height;
  uint8_t *data;

  int color_channels;
  zinc_image_type type;

} zinc_image;

typedef float     zinc_vec2[2];
typedef float     zinc_vec3[3];
typedef float     zinc_vec4[4];

// 'unc' stands for 'unit change'
void zinc_vec4_unc(zinc_image_unit u, zinc_image i, zinc_vec4 v, uint32_t p); 

#endif
