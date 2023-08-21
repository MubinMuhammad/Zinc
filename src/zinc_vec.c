#include "zinc_vec.h"
#include "zinc_types.h"

void zinc_vec2_set(zinc_vec2 v, float a) {
  v[0] = a;
  v[1] = a;
}
void zinc_vec2_add(zinc_vec2 v, float a) {
  v[0] += a;
  v[1] += a;
}
void zinc_vec2_sub(zinc_vec2 v, float a) {
  v[0] -= a;
  v[1] -= a;
}
void zinc_vec2_mul(zinc_vec2 v, float a) {
  v[0] *= a;
  v[1] *= a;
}
void zinc_vec2_div(zinc_vec2 v, float a) {
  v[0] /= a;
  v[1] /= a;
}

void zinc_vec3_set(zinc_vec3 v, float a) {
  v[0] = a;
  v[1] = a;
  v[2] = a;
}
void zinc_vec3_add(zinc_vec3 v, float a) {
  v[0] += a;
  v[1] += a;
  v[2] += a;
}
void zinc_vec3_sub(zinc_vec3 v, float a) {
  v[0] -= a;
  v[1] -= a;
  v[2] -= a;
}
void zinc_vec3_mul(zinc_vec3 v, float a) {
  v[0] *= a;
  v[1] *= a;
  v[2] *= a;
}
void zinc_vec3_div(zinc_vec3 v, float a) {
  v[0] /= a;
  v[1] /= a;
  v[2] /= a;
}

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
void zinc_vec4_set(zinc_vec4 v, float a) {
  v[0] = a;
  v[1] = a;
  v[2] = a;
  v[3] = a;
}
void zinc_vec4_add(zinc_vec4 v, float a) {
  v[0] += a;
  v[1] += a;
  v[2] += a;
  v[3] += a;
}
void zinc_vec4_sub(zinc_vec4 v, float a) {
  v[0] -= a;
  v[1] -= a;
  v[2] -= a;
  v[3] -= a;
}
void zinc_vec4_mul(zinc_vec4 v, float a) {
  v[0] *= a;
  v[1] *= a;
  v[2] *= a;
  v[3] *= a;
}
void zinc_vec4_div(zinc_vec4 v, float a) {
  v[0] /= a;
  v[1] /= a;
  v[2] /= a;
  v[3] /= a;
}
