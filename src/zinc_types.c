#include "zinc_types.h"

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
