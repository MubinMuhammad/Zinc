#ifndef _ZINC_IMAGE_H_
#define _ZINC_IMAGE_H_

#include <stdint.h>
#include <string.h>

#include "zinc_utils.h"

enum zinc_image_type {
  ZINC_JPG = 1,
  ZINC_PNG,
  ZINC_BMP,
  ZINC_TGA
};

// structs
struct zinc_image {
  int width;
  int height;
  uint8_t *data;

  int channels;
  enum zinc_image_type type;
};


struct zinc_image zinc_image_load(const char *path);
void zinc_image_write(struct zinc_image image, const char *name, int jpg_quality);
void zinc_image_free(uint8_t *data);


#endif
