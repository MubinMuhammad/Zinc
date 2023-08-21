#ifndef ZINC_TINT_H
#define ZINC_TINT_H

#include "zinc_types.h"
#include "zinc_vec.h"

void zinc_tint(zinc_image *image, zinc_vec3 const colors);
void zinc_tint_equal(zinc_image *image, float factor);

#endif
