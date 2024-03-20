#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "zinc_global.h"
#include "zinc_utils.h"
#include "zinc_image.h"
#include "zinc_gray.h"
#include "zinc_misc.h"
#include "zinc_tint.h"
#include "zinc_transform.h"

// void zinc_transform_cropcr(zinc_image *image, float *top_left, float *bottom_right) {
//   uint32_t   p = 0;
//   zinc_uvec4 v;
//   
//   zinc_image r = {
//     .width = abs((int)(bottom_right[0] - top_left[0])),
//     .height = abs((int)(bottom_right[1] - top_left[1])),
//     .color_channels = image->color_channels,
//     .type = image->type,
//     .data = malloc(image->width * image->height * image->color_channels)
//   };
//
//   bottom_right[0] += 1;
//   bottom_right[1] += 1;
//
//   uint32_t rp = 0;
//
//   ZINC_PRINT_ACTION("cropping CTR");
//   
//   ZINC_PER_PIXEL_LOOP(image->width, image->height) {
//     zinc_color_to_uvec4(*image, v, p, ZINC_GET);
//
//     if (PIXEL_X > top_left[0] && PIXEL_X < bottom_right[0] &&
//         PIXEL_Y > top_left[1] && PIXEL_Y < bottom_right[1]) {
//       *(r.data + 0 + rp) = v[0] * 255;
//       *(r.data + 1 + rp) = v[1] * 255;
//       *(r.data + 2 + rp) = v[2] * 255;
//       
//       rp += r.color_channels;
//     } 
//
//     zinc_color_to_uvec4(*image, v, p, ZINC_SET);
//     p = (image->width * PIXEL_Y + PIXEL_X) * image->color_channels;
//   }
//
//   *image = r;
// }

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("%s", zinc_help_text);
    return 1;
  } 
  else {
    struct zinc_image   image;
    const char         *image_path;
    char                image_saved_path[100] = {0};
    uint8_t             image_validation = 0;

    /*
     * how the image_validation variable works:
     *  00000000 <- all the bits of the variable.
     *
     *  - 00000001 means no image is provided.
     *  - 00000010 means -o is provided
     * */

    for (int i = 1; i < argc; i++) {
      if (zinc_arg_check("-i", "--image", argv[i])) {
        image_validation |= 1 << 0;
        image_path = argv[i];
        image = zinc_image_load(argv[i + 1]);
        break;
      }
      else if (zinc_arg_check("--help", "-h", argv[i])) {
        printf("%s", zinc_help_text);
        return 0;
      }
    }

    ZINC_ERR((image_validation & 1 << 0) == 0, "image is not provided!\n");

    for (int i = 1; i < argc; i++) {
      if (zinc_arg_check("--gray", "-g", argv[i])) {
        zinc_gray(&image);
      }
      else if (zinc_arg_check("--gray-lumin",  "-gl", argv[i])) {
        zinc_gray_lumin(&image);
      }
      else if (zinc_arg_check("--gray-light",  "-gli", argv[i])) {
        zinc_gray_light(&image);
      }
      else if (zinc_arg_check("--tint",  "-ti", argv[i])) {
        float r = atof(argv[i + 1]);
        float g = atof(argv[i + 2]);
        float b = atof(argv[i + 3]);

        zinc_tint(&image, (float[3]){r, g, b});
      }
      else if (zinc_arg_check("--brightness",  "-br", argv[i])) {
        float rgb = atof(argv[i + 1]);
        zinc_tint_equal(&image, rgb);
      }
      else if (zinc_arg_check("--invert",  "-in", argv[i])) {
        zinc_tint_invert(&image);
      }
      else if (zinc_arg_check("--cropwcr",  "-cwcr", argv[i])) {
        float x1 = atof(argv[i + 1]);
        float y1 = atof(argv[i + 2]);
        float x2 = atof(argv[i + 3]);
        float y2 = atof(argv[i + 4]);

        float r = atof(argv[i + 5]);
        float g = atof(argv[i + 6]);
        float b = atof(argv[i + 7]);

        zinc_transform_cropwcr(&image, 
                               (float[2]){x1, y1},
                               (float[2]){x2, y2},
                               (float[3]){r, g, b});
      }
      // else if (zinc_arg_check("--cropcr",  "-ccr", argv[i])) {
      //   float x1 = atof(argv[i + 1]);
      //   float y1 = atof(argv[i + 2]);
      //   float x2 = atof(argv[i + 3]);
      //   float y2 = atof(argv[i + 4]);
      //
      //   zinc_transform_cropctr(&image, 
      //                          (float[2]){x1, y1},
      //                          (float[2]){x2, y2});
      // }
      else if (zinc_arg_check("--threshold", "-th", argv[i])) {
        float x = atof(argv[i + 1]);
        zinc_threshold(&image, (uint8_t)(x * 255));
      }
      else if (zinc_arg_check("--output", "-o", argv[i])) {
        strcpy(image_saved_path, argv[i + 1]);
        image_validation |= 1 << 1;
      }
    }

    if ((image_validation & 1 << 1) == 0) {
      int si = -1; // starting index
      int ei = -1; // ending index
      for (int i = strlen(image_path) - 1; i >= 0; i--) {
        if (image_path[i] == '.')
          ei = i - 1;
        else if (image_path[i] == '/' || image_path[i] == '\\')
          si = i + 1;

        if (si != -1 && ei != -1)
          break;
      }

      for (int i = si, j = 0; i <= ei; i++, j++) {
        image_saved_path[j] = image_path[i];
      }

      strcat(image_saved_path, "ByZinc");
      if (image.type == ZINC_JPG)
        strcat(image_saved_path, ".jpg\0");
      else if (image.type == ZINC_PNG)
        strcat(image_saved_path, ".png\0");
      else if (image.type == ZINC_BMP)
        strcat(image_saved_path, ".bmp\0");
      else if (image.type == ZINC_TGA)
        strcat(image_saved_path, ".tga\0");
    }


    zinc_image_write(image, image_saved_path, 75);
    zinc_image_free(image.data);
  }

  return 0;
}
