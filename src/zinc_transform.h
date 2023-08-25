#ifndef ZINC_TRANSFORM_H
#define ZINC_TRANSFORM_H

#include "zinc_types.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

// 'wctr' stands for 'without changing the resolution'
void zinc_transform_cropwctr(zinc_image *image, zinc_vec2 top_left, zinc_vec2 bottom_right, zinc_vec3 color);
// 'ctr' stands for 'changing the resolution'
void zinc_transform_cropctr(zinc_image *image, zinc_vec2 top_left, zinc_vec2 bottom_right);

#endif
