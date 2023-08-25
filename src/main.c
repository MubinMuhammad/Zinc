#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "zinc_rw.h"
#include "zinc_transform.h"
#include "zinc_ui.h"
#include "zinc_gray.h"
#include "zinc_tint.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("%s", zinc_help_text);
    return 1;
  } 
  else {
    zinc_image image;
    char       image_path[300];
    bool       image_path_ok = false;


    for (int i = 1; i < argc; i++) {
      if (zinc_image_path_ok(argv[i], strlen(argv[i]))) {
        image_path_ok = true;
        strcpy(image_path, argv[i]);
        image = zinc_image_load(argv[i], strlen(argv[i]));
        break;
      }
      else if (zinc_image_option_check("--help", "-h", argv[i])) {
        printf("%s", zinc_help_text);
        return 1;
      }
      else if (zinc_image_option_check("--version", "-v", argv[i])) {
        printf("Zinc Version: %s \n"
               "This is a free software under the MIT License.\n", ZINC_VERSION);
        return 0;
      }
    }

    if (!image_path_ok) {
      printf("[error]: no image provided!\n");
      return 1;
    }
    else {
      printf("[success]: found image!\n");
    }


    for (int i = 1; i < argc; i++) {
      if (zinc_image_option_check("--gray", "-g", argv[i]))
        zinc_gray(&image);
      if (zinc_image_option_check("--gray-lumin",  "-gl", argv[i]))
        zinc_gray_lumin(&image);
      if (zinc_image_option_check("--gray-light",  "-gli", argv[i]))
        zinc_gray_light(&image);

      if (zinc_image_option_check("--tint",  "-ti", argv[i])) {
        float r = atof(argv[i + 1]);
        float g = atof(argv[i + 2]);
        float b = atof(argv[i + 3]);

        zinc_tint(&image, (zinc_vec3){r, g, b});
      }
      if (zinc_image_option_check("--brightness",  "-br", argv[i])) {
        float rgb = atof(argv[i + 1]);
        zinc_tint_equal(&image, rgb);
      }
      if (zinc_image_option_check("--invert",  "-in", argv[i]))
        zinc_tint_invert(&image);

      if (zinc_image_option_check("--cropwctr",  "-cwctr", argv[i])) {
        float x1 = atof(argv[i + 1]);
        float y1 = atof(argv[i + 2]);
        float x2 = atof(argv[i + 3]);
        float y2 = atof(argv[i + 4]);

        float r = atof(argv[i + 5]);
        float g = atof(argv[i + 6]);
        float b = atof(argv[i + 7]);

        zinc_transform_cropwctr(&image, 
                                (zinc_vec2){x1, y1},
                                (zinc_vec2){x2, y2},
                                (zinc_vec3){r, g, b});
      }
      if (zinc_image_option_check("--cropctr",  "-cctr", argv[i])) {
        float x1 = atof(argv[i + 1]);
        float y1 = atof(argv[i + 2]);
        float x2 = atof(argv[i + 3]);
        float y2 = atof(argv[i + 4]);

        zinc_transform_cropctr(&image, 
                               (zinc_vec2){x1, y1},
                               (zinc_vec2){x2, y2});
      }
    }
    zinc_image_write(image, "ZincImage.jpg", 100);
    zinc_image_free(image);
  }

  return 0;
}
