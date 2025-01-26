// headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// macros
#define ZINC_PER_PIXEL_LOOP(w, h) for(int PIXEL_Y = 0; PIXEL_Y < h; PIXEL_Y++) for(int PIXEL_X = 0; PIXEL_X < w; PIXEL_X++)
#define ZINC_IMAGE_FREE(image) free(image.data)
#define ZINC_PRINT_ACTION(heading) printf("action: applying %s...\n", heading)
#define ZINC_PRINT_SUCCESS(heading) printf("success: %s!\n", heading)
#define ZINC_PRINT_ERROR(heading) printf("error: %s!\n", heading)
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX3(a, b, c) ((a) > (b) && (a) > (c) ? (a) : (b) > (a) && (b) > (c) ? (b) : (c))
#define MIN3(a, b, c) ((a) < (b) && (a) < (c) ? (a) : (b) < (a) && (b) < (c) ? (b) : (c))

// enums
typedef enum zinc_px_rw {
  ZINC_SET,
  ZINC_GET
} zinc_px_rw;

typedef enum zinc_image_type {
  ZINC_JPG = 1,
  ZINC_PNG,
  ZINC_BMP,
  ZINC_TGA
} zinc_image_type;

// structs
typedef struct zinc_image {
  int width;
  int height;
  uint8_t *data;

  int color_channels;
  zinc_image_type type;
} zinc_image;

// typedefs
typedef float     zinc_vec2[2];
typedef float     zinc_vec3[3];
typedef float     zinc_vec4[4];
typedef uint8_t   zinc_uvec4[4];

// global variables
const char *zinc_help_text =
"Pattern:\n"
"  for Linux & Mac:\n"
"    ./zinc [Options] [Image]\n"
"  for Windows:\n"
"    zinc [Options] [Image]\n\n"

"Note:\n"
"  You have to give the parameters after function call.\n"
"  You neigther have to put <> brackets nor commas when <r,g,b> or <x,y> is specified.\n"
"  Just put values and spaces between the parameters.\n"
"    Example: 0.43 0.654 0.12 (r,g,b example)\n"
"             425 645         (x,y example).\n\n"

"  Lastly I used factors instead of percentages. As it\'s more accurate.\n"
"    Example: for brightness, setting factor to 2.0 will double the brightness.\n"
"             for tinting, changing the r value to 1.5 will increase half of the red value.\n\n"

"Options:\n"
"  -v, --version                                    check the version of zinc.\n\n"

"  -h, --help                                       show this help message.\n\n"

"  -o, --output                                     choose a name for the output file.\n\n"
  
"  -g, --gray                                       turn an image gray.\n\n"
  
"  -gl, --gray-lumin                                turn the image gray with rgb values that fits human eyes.\n\n"

"  -gli, --gray-light                               turn the image gray with min,max values.\n\n"
  
"  -ti, --tint <r,g,b>                              tint the image with the <r,g,b> values.\n\n"

"  -br, --brightness <factor>                       change the brightness depending on the percentage.\n\n"

"  -in --invert                                     invert the colors of the image.\n\n"

"  -cwctr, --cropwctr <x1,y1> <x2,y2> <r,g,b>       crop the image without changing the resolution.\n"
"                                                   x1,y1 = top left coordinate. x2,y2 = bottom right coordinate.\n"
"                                                   r,g,b = color for the uncropped areas.\n\n"

"  -cctr, --cropctr <x1,y1> <x2,y2>                 crop the image and change the resolution.\n"
"                                                   x1,y1 = top left coordinate.\n"
"                                                   x2,y2 = bottom right coordinate.\n\n"

"  -th, --threshold <intensity>                     turns the image black and white depending on the threshold.\n"
"                                                   <intensity> ranges from 0 to 255.\n";

// function declarations
zinc_image zinc_image_load_wt(const char *path, zinc_image_type image_type); // here 'wt' stands for 'with type'
zinc_image_type zinc_image_path_ok(const char *path, int path_size);
zinc_image zinc_image_load(const char *path, int path_size);
void zinc_image_write(zinc_image image, const char *name, int jpg_quality);
int zinc_image_option_check(const char *s1, const char *s2, const char *c);
void zinc_color_to_uvec4(zinc_image i, zinc_uvec4 v, uint32_t p, zinc_px_rw sg);
void zinc_gray(zinc_image *image);
void zinc_gray_lumin(zinc_image *image);
void zinc_gray_light(zinc_image *image);
void zinc_threshold(zinc_image *image, uint8_t intensity);
void zinc_tint(zinc_image *image, zinc_vec3 colors);
void zinc_tint_equal(zinc_image *image, float factor);
void zinc_tint_invert(zinc_image *image);
void zinc_transform_cropwctr(zinc_image *image, float *top_left, float *bottom_right, float *color);
void zinc_transform_cropctr(zinc_image *image, float *top_left, float *bottom_right); // here 'ctr' means 'changing the resolution'
void zinc_transform_rotate(zinc_image *image, int rotation_value); // here 'wctr' means 'without changing the resolution'

// function implementation
zinc_image zinc_image_load_wt(const char *path, zinc_image_type image_type) {
  zinc_image i = {
    .type = image_type
  };

  i.data = stbi_load(path, &i.width, &i.height, &i.color_channels, 0);

  if (!i.data) {
    ZINC_PRINT_ERROR("path is invalid or image is corrupted");
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
      ZINC_PRINT_ERROR("failed to write image");
      exit(1);
  }
}

int zinc_image_option_check(const char *s1, const char *s2, const char *c) {
  return 
  strcmp(s1, c) == 0 ||
  strcmp(s2, c) == 0 
  ? 1 : 0;
}

void zinc_color_to_uvec4(zinc_image i, zinc_uvec4 v, uint32_t p, zinc_px_rw sg) {
  if (sg == ZINC_GET) {
    v[0] = *(i.data + 0 + p);
    v[1] = *(i.data + 1 + p);
    v[2] = *(i.data + 2 + p);

    if (i.color_channels == 4)
      v[3] = *(i.data + 3 + p);
  }
  else if (sg == ZINC_SET) {
    *(i.data + 0 + p) = v[0];
    *(i.data + 1 + p) = v[1];
    *(i.data + 2 + p) = v[2];

    if (i.color_channels == 4)
      *(i.data + 3 + p) = v[3];
  } 
}

void zinc_gray(zinc_image *image) {
  uint32_t   p = 0;
  uint8_t    y;
  zinc_uvec4 v;

  ZINC_PRINT_ACTION("grayscale avarage");

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_color_to_uvec4(*image, v, p, ZINC_GET);
    y = (v[0] + v[1] + v[2]) / 3;

    v[0] = y;
    v[1] = y;
    v[2] = y;
 
    zinc_color_to_uvec4(*image, v, p, ZINC_SET);
    p = (image->width * PIXEL_Y + PIXEL_X) * image->color_channels;
  }
}


void zinc_gray_lumin(zinc_image *image) {
  uint32_t   p = 0;
  uint8_t    y;
  zinc_uvec4 v;

  ZINC_PRINT_ACTION("grayscale luminosity");

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_color_to_uvec4(*image, v, p, ZINC_GET);

    y = (v[0] * 0.2126 + 
         v[1] * 0.7152 + 
         v[2] * 0.0722 ) / 3;

    v[0] = y;
    v[1] = y;
    v[2] = y;

    zinc_color_to_uvec4(*image, v, p, ZINC_SET);
    p = (image->width * PIXEL_Y + PIXEL_X) * image->color_channels;
  }
}

void zinc_gray_light(zinc_image *image) {
  uint32_t    p = 0;
  uint8_t     y, mx, mn;
  zinc_uvec4  v;

  ZINC_PRINT_ACTION("grayscale lightness");
  
  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_color_to_uvec4(*image, v, p, ZINC_GET);
    
    mx = MAX3(v[0], v[1], v[2]);
    mn = MIN3(v[0], v[1], v[2]);

    y = (mx + mn) / 3;

    v[0] = y;
    v[1] = y;
    v[2] = y;

    zinc_color_to_uvec4(*image, v, p, ZINC_SET);
    p = (image->width * PIXEL_Y + PIXEL_X) * image->color_channels;
  }
}

void zinc_threshold(zinc_image *image, uint8_t intensity) {
  zinc_gray(image);

  uint32_t   p = 0;
  zinc_uvec4 v;

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_color_to_uvec4(*image, v, p, ZINC_GET);

    if (v[0] > intensity) {
      v[0] = 255;
      v[1] = 255;
      v[2] = 255;
    }
    else {
      v[0] = 0;
      v[1] = 0;
      v[2] = 0;
    }

    zinc_color_to_uvec4(*image, v, p, ZINC_SET);
    p = (image->width * PIXEL_Y + PIXEL_X) * image->color_channels;
  }
}

void zinc_tint(zinc_image *image, zinc_vec3 colors) {
  uint32_t   p = 0, c;
  zinc_uvec4 v;

  ZINC_PRINT_ACTION("tint");

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_color_to_uvec4(*image, v, p, ZINC_GET);

    // if c is set after v[0] *= colors[0] then c might be 0
    c = (uint32_t)v[0] * colors[0];
    c > 255 ? v[0] = 255 : (v[0] *= colors[0]);

    // if c is set after v[1] *= colors[1] then c might be 0
    c = (uint32_t)v[1] * colors[1];
    c > 255 ? v[1] = 255 : (v[1] *= colors[1]);

    // if c is set after v[2] *= colors[2] then c might be 0
    c = (uint32_t)v[2] * colors[2];
    c > 255 ? v[2] = 255 : (v[2] *= colors[2]);

    zinc_color_to_uvec4(*image, v, p, ZINC_SET);
    p = (image->width * PIXEL_Y + PIXEL_X) * image->color_channels;
  }
}

void zinc_tint_equal(zinc_image *image, float factor) {
  zinc_tint(image, (zinc_vec3){factor, factor, factor});
}

void zinc_tint_invert(zinc_image *image) {
  uint32_t   p = 0;
  zinc_uvec4 v;

  ZINC_PRINT_ACTION("invert");

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_color_to_uvec4(*image, v, p, ZINC_GET);

    v[0] = 255 - v[0];
    v[1] = 255 - v[1];
    v[2] = 255 - v[2];

    zinc_color_to_uvec4(*image, v, p, ZINC_SET);
    p = (image->width * PIXEL_Y + PIXEL_X) * image->color_channels;
  }
}

void zinc_transform_cropwctr(zinc_image *image, float *top_left, float *bottom_right, float *color) {
  uint32_t   p = 0;
  zinc_uvec4 v;

  ZINC_PRINT_ACTION("cropping WCTR");

  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_color_to_uvec4(*image, v, p, ZINC_GET);

    if (!(PIXEL_X > top_left[0] && PIXEL_X < bottom_right[0] &&
          PIXEL_Y > top_left[1] && PIXEL_Y < bottom_right[1])) {
      v[0] = (color[0] * 255);
      v[1] = (color[1] * 255);
      v[2] = (color[2] * 255);
    } 

    zinc_color_to_uvec4(*image, v, p, ZINC_SET);
    p = (image->width * PIXEL_Y + PIXEL_X) * image->color_channels;
  }
}

void zinc_transform_cropctr(zinc_image *image, float *top_left, float *bottom_right) {
  uint32_t   p = 0;
  zinc_uvec4 v;
  
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

  ZINC_PRINT_ACTION("cropping CTR");
  
  ZINC_PER_PIXEL_LOOP(image->width, image->height) {
    zinc_color_to_uvec4(*image, v, p, ZINC_GET);

    if (PIXEL_X > top_left[0] && PIXEL_X < bottom_right[0] &&
        PIXEL_Y > top_left[1] && PIXEL_Y < bottom_right[1]) {
      *(r.data + 0 + rp) = v[0] * 255;
      *(r.data + 1 + rp) = v[1] * 255;
      *(r.data + 2 + rp) = v[2] * 255;
      
      rp += r.color_channels;
    } 

    zinc_color_to_uvec4(*image, v, p, ZINC_SET);
    p = (image->width * PIXEL_Y + PIXEL_X) * image->color_channels;
  }

  *image = r;
}

void zinc_transform_rotate(zinc_image *image, int rotation_value) {
  uint32_t   p = 0;
  zinc_uvec4 v;

  zinc_image r = {
    .width = image->height,
    .height = image->width,
    .color_channels = image->color_channels,
    .type = image->type,
    .data = malloc(image->width * image->height * image->color_channels)
  };

  printf("width: %d, height: %d", image->width, image->height);

  int rxy = 0;

  switch (rotation_value) {
    case 0:
    for (int PIXEL_X = 0; PIXEL_X < image->width; PIXEL_X++) {
      for (int PIXEL_Y = image->height - 1; PIXEL_Y >= 0; PIXEL_Y--) {
        zinc_color_to_uvec4(*image, v, p, ZINC_GET);

        *(r.data + 0 + rxy) = v[0];
        *(r.data + 1 + rxy) = v[1];
        *(r.data + 2 + rxy) = v[2];

        if (image->color_channels == 4)
          *(r.data + 3 + rxy) = v[3];

        rxy += image->color_channels;
        p = (image->width * PIXEL_Y + PIXEL_X) * image->color_channels;
      }
    }
    *image = r;
    break;
  };
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("%s", zinc_help_text);
    return 1;
  } 
  else {
    zinc_image   image;
    const char  *image_path;
    char         image_saved_path[100] = {0};
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
    }

    if (!image_path_ok) {
      ZINC_PRINT_ERROR("no image provided");
      return 1;
    }
    else {
      ZINC_PRINT_SUCCESS("image is successfully loaded");
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
      else if (zinc_image_option_check("--threshold", "-th", argv[i])) {
        uint8_t x = atoi(argv[i + 1]);
        zinc_threshold(&image, x);
      }
      else if (zinc_image_option_check("--rotate", "-rt", argv[i])) {
        int x;
        if (strcmp(argv[i + 1], "right"))
          x = 0;
        
        zinc_transform_rotate(&image, x);
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
    ZINC_PRINT_SUCCESS("done");
    ZINC_IMAGE_FREE(image);
  }

  return 0;
}
