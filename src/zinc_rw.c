#include "zinc_rw.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// 'wt' stands for 'with type'
static zinc_image zinc_image_load_wt(const char *path, zinc_image_type image_type) {
  zinc_image i = {
    .type = image_type
  };

  i.data = stbi_load(path, 
                     &i.width, &i.height,
                     &i.color_channels,
                     0);

  if (!i.data) {
    printf("[error]: path is invalid or image is corrupted!\n");
    exit(1);
  }

  return i;
}

zinc_image zinc_image_load(const char *path, int path_size) {
  zinc_image i;
  
  if (strcmp(path + (path_size - 3), "jpg") == 0 || 
      strcmp(path + (path_size - 4), "jpeg"))
    i = zinc_image_load_wt(path, ZINC_JPG);
  else if (strcmp(path + (path_size - 3), "png") == 0)
    i = zinc_image_load_wt(path, ZINC_PNG);
  else if (strcmp(path + (path_size - 3), "bmp") == 0)
    i = zinc_image_load_wt(path, ZINC_BMP);
  else if (strcmp(path + (path_size - 3), "tga") == 0)
    i = zinc_image_load_wt(path, ZINC_TGA);

  return i;
}

void zinc_image_write(zinc_image image, const char *name, int jpg_quality) {
  switch (image.type) {
    case ZINC_JPG:
      stbi_write_jpg(name, 
                     image.width, image.height, 
                     image.color_channels, 
                     image.data, jpg_quality);
      break;
    case ZINC_PNG:
      stbi_write_png(name,
                     image.width, image.height,
                     image.color_channels,
                     image.data, 
                     image.width * image.color_channels);
      break;
    case ZINC_BMP:
      stbi_write_bmp(name,
                     image.width, image.height,
                     image.color_channels, image.data);
      break;
    case ZINC_TGA:
      stbi_write_tga(name,
                     image.width, image.height,
                     image.color_channels, image.data);
    default:
      printf("[error]: failed to write image!");
      exit(1);
  }
}

int zinc_image_path_ok(const char *path, int path_size) {
  return
  strcmp(path + (path_size - 4), "jpeg") == 0 ||
  strcmp(path + (path_size - 3), "jpg")  == 0 ||
  strcmp(path + (path_size - 3), "png")  == 0 ||
  strcmp(path + (path_size - 3), "bmp")  == 0 ||
  strcmp(path + (path_size - 3), "tga")  == 0 
  ? 1 : 0;
}

int zinc_image_option_check(const char *s1, const char *s2, const char *c) {
  return 
  strcmp(s1, c) == 0 ||
  strcmp(s2, c) == 0 
  ? 1 : 0;
}
