#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


// -------------------------- zinc types --------------------------  //
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
  ZINC_JPG = 1,
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

// here 'unc' means 'unit change'
void zinc_vec4_unc(zinc_image_unit u, zinc_image i, float *v, uint32_t p) {
  if (u == ZINC_RGB_TO_NORMALIZED) {
    v[0] = (float)*(i.data + 0 + p) / 255.0f;
    v[1] = (float)*(i.data + 1 + p) / 255.0f;
    v[2] = (float)*(i.data + 2 + p) / 255.0f;

    if (i.color_channels == 4)
      v[3] = (float)*(i.data + 3 + p) / 255.0f;
  }
  else if (u == ZINC_NORMALIZED_TO_RGB) {
    *(i.data + 0 + p) = v[0] * 255;
    *(i.data + 1 + p) = v[1] * 255;
    *(i.data + 2 + p) = v[2] * 255;

    if (i.color_channels == 4)
      *(i.data + 3 + p) = v[3] * 255;
  }
}
// -------------------------- zinc types --------------------------  // END


// -------------------------- zinc ui --------------------------  //
#define ZINC_VERSION "1.0.0 Code: 20230831" 

const char *zinc_help_text = 
"Pattern:\n"
"  for Linux & Mac:\n"
"    ./zinc [Options] [Image]\n"
"  for Windows:\n"
"    zinc [Options] [Image]\n\n"

"Note:\n"
"  You have to give the parameters after function call.\n"
"  You neigther have to put <> brackets nor commas when <r,g,b> or <x,y> is specified. Just put values and spaces between the parameters.\n"
"    Example: 0.43 0.654 0.12 (r,g,b example)\n"
"             425 645   (x,y example).\n"
"  Lastly we use factors instead of percentages. As it\'s more accurate.\n"
"    Example: for brightness, setting factor to 2.0 will double the brightness.\n"
"             for tinting, changing the r value to 1.5 will increase half of the red value.\n\n"

"Options:\n"
"  -v, --version                                    check the version of zinc.\n"
"  -h, --help                                       show this help message.\n"
"  -o, --output                                     choose a name for the output file.\n"
"  -g, --gray                                       turn an image gray.\n"
"  -gl, --gray-lumin                                turn the image gray with rgb values that fits human eyes.\n"
"  -gli, --gray-light                               turn the image gray with min,max values.\n"
"  -ti, --tint <r,g,b>                              tint the image with the <r,g,b> values.\n"
"  -br, --brightness <factor>                       change the brightness depending on the percentage.\n"
"  -in --invert                                     invert the colors of the image.\n"
"  -cwctr, --cropwctr <x1,y1> <x2,y2> <r,g,b>       crop the image without changing the resolution. x1,y1 = top left coordinate. x2,y2 = bottom right coordinate. r,g,b = color for the uncropped areas.\n"
"  -cctr, --cropctr <x1,y1> <x2,y2>                 crop the image and change the resolution. x1,y1 = top left coordinate. x2,y2 = bottom right coordinate.\n"
"  more soon...\n\n";

void zinc_print_action(const char *heading) {
  printf("action: applying %s...\n", heading);
}
void zinc_print_success(const char *heading) {
  printf("success: %s!\n", heading);
}
void zinc_print_error(const char *heading) {
  printf("error: %s!\n", heading);
}
// -------------------------- zinc ui --------------------------  // END




// -------------------------- zinc rw --------------------------  //
#define zinc_image_free(image) free(image.data)

// 'wt' stands for 'with type'
zinc_image zinc_image_load_wt(const char *path, zinc_image_type image_type) {
  zinc_image i = {
    .type = image_type
  };

  i.data = stbi_load(path, 
                     &i.width, &i.height,
                     &i.color_channels,
                     0);

  if (!i.data) {
    zinc_print_error("path is invalid or image is corrupted");
    exit(1);
  }

  return i;
}

zinc_image_type zinc_image_path_ok(const char *path, int path_size) {
  return
  strcmp(path + (path_size - 4), "jpeg") == 0 ||
  strcmp(path + (path_size - 3), "jpg")  == 0 ? ZINC_JPG :
  strcmp(path + (path_size - 3), "png")  == 0 ? ZINC_PNG :
  strcmp(path + (path_size - 3), "bmp")  == 0 ? ZINC_BMP :
  strcmp(path + (path_size - 3), "tga")  == 0 ? ZINC_TGA : 0;
}

zinc_image zinc_image_load(const char *path, int path_size) {
  zinc_image i;
  
  i = zinc_image_load_wt(path, zinc_image_path_ok(path, path_size));

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
                     image.color_channels, image.data); default:
      zinc_print_error("failed to write image");
      exit(1);
  }
}

int zinc_image_option_check(const char *s1, const char *s2, const char *c) {
  return 
  strcmp(s1, c) == 0 ||
  strcmp(s2, c) == 0 
  ? 1 : 0;
}
// -------------------------- zinc rw --------------------------  // END


// -------------------------- zinc gray --------------------------  //
void zinc_gray(zinc_image *image) {
  uint32_t   p = 0;
  float      y;
  zinc_vec4  v;

  zinc_print_action("grayscale avarage");

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

void zinc_gray_lumin(zinc_image *image) {
  uint32_t  p = 0;
  float     y;
  zinc_vec4 v;

  zinc_print_action("grayscale luminosity");

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_vec4_unc(ZINC_RGB_TO_NORMALIZED, *image, v, p);

    y = (v[0] * 0.2126 + 
         v[1] * 0.7152 + 
         v[2] * 0.0722 ) / 3;

    v[0] = y;
    v[1] = y;
    v[2] = y;

    zinc_vec4_unc(ZINC_NORMALIZED_TO_RGB, *image, v, p);
    p += image->color_channels;
  }
}

void zinc_gray_light(zinc_image *image) {
  uint32_t  p = 0;
  float     y;
  float     mx;
  float     mn;
  zinc_vec4 v;

  zinc_print_action("grayscale lightness");
  
  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_vec4_unc(ZINC_RGB_TO_NORMALIZED, *image, v, p);
    
    mx = max3(v[0], v[1], v[2]);
    mn = min3(v[0], v[1], v[2]);

    y = (mx + mn) / 3;

    v[0] = y;
    v[1] = y;
    v[2] = y;

    zinc_vec4_unc(ZINC_NORMALIZED_TO_RGB, *image, v, p);
    p += image->color_channels;
  }
}
// -------------------------- zinc gray --------------------------  // END


// -------------------------- zinc tint --------------------------  //
void zinc_tint(zinc_image *image, float *colors) {
  uint32_t  p = 0;
  zinc_vec4 v;

  zinc_print_action("tint");

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_vec4_unc(ZINC_RGB_TO_NORMALIZED, *image, v, p);

    v[0] *= colors[0];
    v[1] *= colors[1];
    v[2] *= colors[2];

    v[0] > 1.0f ? v[0] = 1.0f : 0;
    v[1] > 1.0f ? v[1] = 1.0f : 0;
    v[2] > 1.0f ? v[2] = 1.0f : 0;

    zinc_vec4_unc(ZINC_NORMALIZED_TO_RGB, *image, v, p);
    p += image->color_channels;

  }
}

void zinc_tint_equal(zinc_image *image, float factor) {
  uint32_t  p = 0;
  zinc_vec4 v;

  zinc_print_action("brightness");

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_vec4_unc(ZINC_RGB_TO_NORMALIZED, *image, v, p);

    v[0] *= factor;
    v[1] *= factor;
    v[2] *= factor;

    v[0] > 1.0f ? v[0] = 1.0f : 0;
    v[1] > 1.0f ? v[1] = 1.0f : 0;
    v[2] > 1.0f ? v[2] = 1.0f : 0;

    zinc_vec4_unc(ZINC_NORMALIZED_TO_RGB, *image, v, p);
    p += image->color_channels;
  }
}

void zinc_tint_invert(zinc_image *image) {
  uint32_t  p = 0;
  zinc_vec4 v;

  zinc_print_action("invert");

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_vec4_unc(ZINC_RGB_TO_NORMALIZED, *image, v, p);

    v[0] = 1.0f - v[0];
    v[1] = 1.0f - v[1];
    v[2] = 1.0f - v[2];

    zinc_vec4_unc(ZINC_NORMALIZED_TO_RGB, *image, v, p);
    p += image->color_channels;
  }
}
// -------------------------- zinc tint --------------------------  // END


// -------------------------- zinc transform --------------------------  //
// here 'wctr' means 'without changing the resolution'
void zinc_transform_cropwctr(zinc_image *image, float *top_left, float *bottom_right, float *color) {
  uint32_t  p = 0;
  zinc_vec4 v;

  zinc_print_action("cropping WCTR");

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

// here 'ctr' means 'changing the resolution'
void zinc_transform_cropctr(zinc_image *image, float *top_left, float *bottom_right) {
  uint32_t  p = 0;
  zinc_vec4 v;
  
  zinc_image r = {
    .width = abs((int)(bottom_right[0] - top_left[0])),
    .height = abs((int)(bottom_right[1] - top_left[1])),
    .color_channels = image->color_channels,
    .type = image->type,
    .data = malloc(image->width * image->height * image->color_channels)
  };

  bottom_right[0] += 1;
  bottom_right[1] += 1;

  uint32_t rp = 0;

  zinc_print_action("cropping CTR");
  
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

void zinc_transform_rotate(zinc_image *image, const char *side) {
  int oriantation = strcmp(side, "right") == 0 ? 1 :
                    strcmp(side, "left")  == 0 ? 2 : 3;

  zinc_image r = {
    .width = oriantation == 1 || oriantation == 2 ? image->height : image->width,
    .height = oriantation == 1 || oriantation == 2 ? image->width : image->height,
    .color_channels = image->color_channels,
    .type = image->type,
    .data = malloc(image->width * image->height * image->color_channels)
  };
  
  int PIXEL_X = oriantation == 1 ? 0 : image->width - 1;
  int PIXEL_Y = oriantation == 1 ? image->height - 1 : 0;


}
// -------------------------- zinc transform --------------------------  // END


int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("%s", zinc_help_text);
    return 1;
  } 
  else {
    zinc_image   image;
    const char  *image_path;
    char         image_saved_path[100];
    bool         image_path_ok = false;
    bool         image_default_path = true;


    for (int i = 1; i < argc; i++) {
      if (zinc_image_path_ok(argv[i], strlen(argv[i]))) {
        image_path_ok = true;
        image_path = argv[i];
        image = zinc_image_load(argv[i], strlen(argv[i]));
        break;
      }
      else if (zinc_image_option_check("--help", "-h", argv[i])) {
        printf("%s", zinc_help_text);
        return 0;
      }
      else if (zinc_image_option_check("--version", "-v", argv[i])) {
        printf("Zinc Version: %s \n"
               "This is a free software under the MIT License.\n", ZINC_VERSION);
        return 0;
      }
    }


    if (!image_path_ok) {
      zinc_print_error("no image provided");
      return 1;
    }
    else {
      zinc_print_success("image is successfully loaded");
    }


    for (int i = 1; i < argc; i++) {
      if (zinc_image_option_check("--gray", "-g", argv[i]))
        zinc_gray(&image);
      else if (zinc_image_option_check("--gray-lumin",  "-gl", argv[i]))
        zinc_gray_lumin(&image);
      else if (zinc_image_option_check("--gray-light",  "-gli", argv[i]))
        zinc_gray_light(&image);

      else if (zinc_image_option_check("--tint",  "-ti", argv[i])) {
        float r = atof(argv[i + 1]);
        float g = atof(argv[i + 2]);
        float b = atof(argv[i + 3]);

        zinc_tint(&image, (zinc_vec3){r, g, b});
      }
      else if (zinc_image_option_check("--brightness",  "-br", argv[i])) {
        float rgb = atof(argv[i + 1]);
        zinc_tint_equal(&image, rgb);
      }
      else if (zinc_image_option_check("--invert",  "-in", argv[i]))
        zinc_tint_invert(&image);

      else if (zinc_image_option_check("--cropwctr",  "-cwctr", argv[i])) {
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
      else if (zinc_image_option_check("--cropctr",  "-cctr", argv[i])) {
        float x1 = atof(argv[i + 1]);
        float y1 = atof(argv[i + 2]);
        float x2 = atof(argv[i + 3]);
        float y2 = atof(argv[i + 4]);

        zinc_transform_cropctr(&image, 
                               (zinc_vec2){x1, y1},
                               (zinc_vec2){x2, y2});
      }
      else if (zinc_image_option_check("--output", "-o", argv[i])) {
        strcpy(image_saved_path, argv[i + 1]);
        image_default_path = false;
      }
    }


    if (image_default_path) {
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
    }

    strcat(image_saved_path, "ByZinc");
    
    if (zinc_image_path_ok(image_path, strlen(image_path)) == ZINC_JPG)
      strcat(image_saved_path, ".jpg");
    else if (zinc_image_path_ok(image_path, strlen(image_path)) == ZINC_PNG)
      strcat(image_saved_path, ".png");
    else if (zinc_image_path_ok(image_path, strlen(image_path)) == ZINC_BMP)
      strcat(image_saved_path, ".bmp");
    else if (zinc_image_path_ok(image_path, strlen(image_path)) == ZINC_TGA)
      strcat(image_saved_path, ".tga");


    zinc_image_write(image, image_saved_path, 75);
    zinc_print_success("done");
    zinc_image_free(image);
  }

  return 0;
}
