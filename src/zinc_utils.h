#ifndef _ZINC_UTILS_H_
#define _ZINC_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZINC_ERR(e, msg)                                               \
  do {                                                                 \
    if (e) {                                                           \
      fprintf(stderr, "[error][%d:%s]: %s", __LINE__, __FILE__, msg);  \
      exit(-1);                                                        \
    }                                                                  \
  } while(0)

#define MAX(a, b)                   ((a) > (b) ? (a) : (b))
#define MIN(a, b)                   ((a) < (b) ? (a) : (b))
#define CLAMP(x, mn, mx)            ((x < mn ? mn : (x > mx ? mx : x)))

int zinc_arg_check(const char *s1, const char *s2, const char *c);

#endif
