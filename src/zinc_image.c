#include "zinc_image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stb_image_write.h"

static enum zinc_image_type zinc_image_path_ok(const char *path) {
  int path_size = strlen(path);

  return
  strcmp(path + (path_size - 4), "jpeg") == 0 ||
  strcmp(path + (path_size - 3), "jpg")  == 0 ? ZINC_JPG :
  strcmp(path + (path_size - 3), "png")  == 0 ? ZINC_PNG :
  strcmp(path + (path_size - 3), "bmp")  == 0 ? ZINC_BMP :
  strcmp(path + (path_size - 3), "tga")  == 0 ? ZINC_TGA : 0;
}

struct zinc_image zinc_image_load(const char *path) {
  struct zinc_image i = {
    .type = zinc_image_path_ok(path)
  };

  i.data = stbi_load(path, &i.width, &i.height, &i.channels, 0);

  ZINC_ERR(i.data == NULL, "Failed to load image!\n");

  return i;
}

void zinc_image_write(struct zinc_image image, const char *name, int jpg_quality) {
  switch (image.type) {
    case ZINC_JPG:
      stbi_write_jpg(name, 
                     image.width, image.height, 
                     image.channels, 
                     image.data, jpg_quality);
      break;
    case ZINC_PNG:
      stbi_write_png(name,
                     image.width, image.height,
                     image.channels,
                     image.data, 
                     image.width * image.channels);
      break;
    case ZINC_BMP:
      stbi_write_bmp(name,
                     image.width, image.height,
                     image.channels, image.data);
      break;
    case ZINC_TGA:
      stbi_write_tga(name,
                     image.width, image.height,
                     image.channels, image.data);
      break;
    default:
      ZINC_ERR(1, "Failed to write image!");
  }
}

void zinc_image_free(uint8_t *data) {
  stbi_image_free(data);
}
