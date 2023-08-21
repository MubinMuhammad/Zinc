#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "zinc_rw.h"
#include "zinc_ui.h"
#include "zinc_gray.h"

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
    }

    if (!image_path_ok) {
      printf("[error]: no image provided!");
      return 1;
    }


    for (int i = 1; i < argc; i++) {
      if (zinc_image_option_ok("--gray", "-g", argv[i]))
        zinc_gray(&image);
      if (zinc_image_option_ok("--gray-correc", "-gc", argv[i]))
        zinc_gray_correc(&image);
      if (zinc_image_option_ok("--gray-lumin",  "-glu", argv[i]))
        zinc_gray_lumin(&image);
      if (zinc_image_option_ok("--gray-light",  "-gli", argv[i]))
        zinc_gray_light(&image);
    }
    zinc_image_write(image, "ZincImage.jpg", 100);
    zinc_image_free(image);
  }

  return 0;
}
