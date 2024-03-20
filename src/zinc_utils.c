#include "zinc_utils.h"

int zinc_arg_check(const char *s1, const char *s2, const char *c) {
  return 
  strcmp(s1, c) == 0 ||
  strcmp(s2, c) == 0 
  ? 1 : 0;
}
