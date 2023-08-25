#ifndef ZINC_RW_H
#define ZINC_RW_H

#include "zinc_types.h"
#include <string.h>

#define zinc_image_free(image) free(image.data)

zinc_image zinc_image_load(const char *path, int path_size);
void zinc_image_write(zinc_image image, const char *name, int jpg_quality);
int zinc_image_path_ok(const char *path, int path_size);
int zinc_image_option_check(const char *s1, const char *s2, const char *c);

#endif
