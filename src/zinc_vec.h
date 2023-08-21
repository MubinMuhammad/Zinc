#ifndef ZINC_VEC_H
#define ZINC_VEC_H

#include "zinc_types.h"
#include <stddef.h>

typedef float     zinc_vec2[2];
typedef float     zinc_vec3[3];
typedef float     zinc_vec4[4];

void zinc_vec2_set(zinc_vec2 v, float a);
void zinc_vec2_add(zinc_vec2 v, float a);
void zinc_vec2_sub(zinc_vec2 v, float a);
void zinc_vec2_mul(zinc_vec2 v, float a);
void zinc_vec2_div(zinc_vec2 v, float a);

void zinc_vec3_set(zinc_vec3 v, float a);
void zinc_vec3_add(zinc_vec3 v, float a);
void zinc_vec3_sub(zinc_vec3 v, float a);
void zinc_vec3_mul(zinc_vec3 v, float a);
void zinc_vec3_div(zinc_vec3 v, float a);

// 'unc' stands for 'unit change'
void zinc_vec4_unc(zinc_image_unit u, zinc_image i, zinc_vec4 v, uint32_t p); 
void zinc_vec4_set(zinc_vec4 v, float a);
void zinc_vec4_add(zinc_vec4 v, float a);
void zinc_vec4_sub(zinc_vec4 v, float a);
void zinc_vec4_mul(zinc_vec4 v, float a);
void zinc_vec4_div(zinc_vec4 v, float a);

#endif
